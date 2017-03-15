/*
 * Copyright (C) 2017 nikoapos
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * @file modules/BMP180.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_BMP180_H
#define PIHWCTRL_BMP180_H

#include <cstdint>
#include <memory>
#include <chrono>
#include <mutex>
#include <atomic>
#include <PiHWCtrl/HWInterfaces/AnalogInput.h>
#include <PiHWCtrl/utils/EncapsulatedObservable.h>

namespace PiHWCtrl {

/**
 * @class BMP180
 * 
 * @brief
 * Class for controlling the BMP180 pressure and temperature sensor
 * 
 * @details
 * The BMP180 can measure both temperature and pressure. This class handles the
 * communication with the sensor and it computes the altitude based on the
 * measured pressure and the sea level pressure.
 * 
 * The sea level pressure can either be given (as a constructor parameter or by
 * using the setSeaLevelPressure() method), or it can be calibrated using a
 * known current altitude, by using the method calibrateSeaLevelPressure().
 * 
 * This class provides access to both the raw uncompensated measurements from the
 * sensors and the true values after applying the calibration, via three ways:
 * 
 * - By direct access via the dedicated methods
 * - By providing AnalogInput implementations for each measurement to be used
 *   for pull implementations (suitable to be passed as parameters to objects
 *   that expect AnalogInputs)
 * - By accepting Observers to be notified for each measurement to be used for
 *   push implementations
 * 
 * Note that if the AnalogInput implementations method is used, the lifetime of
 * the BMP180 object itself must be longer than the ones of the AnalogInputs.
 */
class BMP180 {
  
public:
  
  /// The different sampling accuracy modes for the pressure measurements
  enum class PressureMode {
    ULTRA_LOW_POWER,       ///< Conversion time  4.5ms. RMS 0.06hPa - 0.5m
    STANDARD,              ///< Conversion time  7.5ms, RMS 0.05hPa - 0.4m
    HIGH_RESOLUTION,       ///< Conversion time 13.5ms, RMS 0.04hPa - 0.3m
    ULTRA_HIGH_RESOLUTION  ///< Conversion time 25.5ms, RMS 0.03hPa - 0.25m
  };
  
  /**
   * @brief Creates a new BMP180 instance
   * 
   * @details
   * This call will perform a soft reset to the device. Only a single instance
   * of the BMP180 can exist at a time. If it needs to be accessed from multiple
   * places, the result of this call should be stored in a shared pointer and
   * copied around.
   * 
   * @param mode
   *    The pressure sampling accuracy mode
   * @param sea_level_pressure
   *    The sea level pressure to use for altitude computation
   * @return
   *    The BMP180 instance
   * @throws ModuleAlreadyInUse
   *    If there is already an instance of the BMP180 class controlling the device
   * @throws I2CDeviceConnectionFailure
   *    If the connection to the device fails
   * @throws I2CWrongModule
   *    If the connected device is not a BMP180
   */
  static std::unique_ptr<BMP180> factory(PressureMode mode=PressureMode::STANDARD,
                                         float sea_level_pressure=1020);
  
  /// Allows other BMP180 instances to be created
  virtual ~BMP180();
  
  /// Returns the uncompensated value of the temperature
  std::uint16_t readRawTemperature();
  
  /// Returns an AnalogInput for accessing the uncompensated temperature
  std::unique_ptr<AnalogInput<std::uint16_t>> rawTemperatureAnalogInput();
  
  /// Adds an observer which will be notified for uncompensated temperature values
  void addRawTemperatureObserver(std::shared_ptr<Observer<std::uint16_t>> observer);
  
  /// Returns the calibrated value of the temperature
  float readTemperature();
  
  /// Returns an AnalogInput for accessing the calibrated temperature
  std::unique_ptr<AnalogInput<float>> temperatureAnalogInput();
  
  /// Adds an observer which will be notified for calibrated temperature values
  void addTemperatureObserver(std::shared_ptr<Observer<float>> observer);
  
  /// Returns the uncompensated value of the pressure
  std::uint32_t readRawPressure();
  
  /// Returns an AnalogInput for accessing the uncompensated pressure
  std::unique_ptr<AnalogInput<std::uint16_t>> rawPressureAnalogInput();
  
  /// Adds an observer which will be notified for uncompensated pressure values
  void addRawPressureObserver(std::shared_ptr<Observer<std::uint32_t>> observer);
  
  /// Returns the calibrated value of the pressure
  float readPressure();
  
  /// Returns an AnalogInput for accessing the calibrated pressure
  std::unique_ptr<AnalogInput<float>> pressureAnalogInput();
  
  /// Adds an observer which will be notified for calibrated pressure values
  void addPressureObserver(std::shared_ptr<Observer<float>> observer);
  
  // Returns the computed altitude
  float readAltitude();
  
  /// Returns an AnalogInput for accessing the altitude
  std::unique_ptr<AnalogInput<float>> altitudeAnalogInput();
  
  /// Adds an observer which will be notified for altitude values
  void addAltitudeObserver(std::shared_ptr<Observer<float>> observer);
  
  /// Returns the sea level pressure used for computing the altitude
  float getSeaLevelPressure();
  
  /// Sets the sea level pressure to a fixed user value
  void setSeaLevelPressure(float pressure);
  
  /// Calibrates the sea level pressure assuming that the sensor is at the
  /// given altitude
  float calibrateSeaLevelPressure(float altitude);
  
  /// Start continuous measurement mode, which notifies the observers.
  void start();
  
  /// Stop the continuous measurement mode
  void stop();
  
private:
  
  BMP180(PressureMode mode, float sea_level_pressure);
  
  std::int32_t computeB5(std::uint16_t ut);
  float computeRealTemperature(std::uint16_t ut, std::int32_t b5);
  float computeRealPressure(std::uint16_t ut, std::uint32_t up);
  float computeAltitude(float pressure);
  
  PressureMode m_mode;
  float m_sea_level_pressure;
  std::int16_t m_ac1;
  std::int16_t m_ac2;
  std::int16_t m_ac3;
  std::uint16_t m_ac4;
  std::uint16_t m_ac5;
  std::uint16_t m_ac6;
  std::int16_t m_b1;
  std::int16_t m_b2;
  std::int16_t m_mb;
  std::int16_t m_mc;
  std::int16_t m_md;
  std::uint16_t m_last_temperature;
  std::chrono::time_point<std::chrono::steady_clock> m_last_temperature_timestamp;
  EncapsulatedObservable<std::uint16_t> m_raw_temperature_observable;
  EncapsulatedObservable<float> m_temperature_observable;
  EncapsulatedObservable<std::uint32_t> m_raw_pressure_observable;
  EncapsulatedObservable<float> m_pressure_observable;
  EncapsulatedObservable<float> m_altitude_observable;
  mutable std::mutex m_mutex;
  std::atomic<bool> m_observing {false};
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_BMP180_H */

