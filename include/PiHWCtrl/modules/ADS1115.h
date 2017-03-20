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
 * @file PiHWCtrl/modules/ADS1115.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_MODULES_ADS1115_H
#define PIHWCTRL_MODULES_ADS1115_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <map>
#include <atomic>
#include <PiHWCtrl/HWInterfaces/AnalogInput.h>
#include <PiHWCtrl/utils/EncapsulatedObservable.h>

namespace PiHWCtrl {

/**
 * @class ADS1115
 * 
 * @brief
 * Class for controlling the ADS1115 analog to digital converter (ADC)
 * 
 * @details
 * The ADS1115 has four analog inputs and it capable to measure the following
 * differential voltages:
 * 
 * - IN 0 - IN 1
 * - IN 0 - IN 3
 * - IN 1 - IN 3
 * - IN 2 - IN 3
 * - IN 0 - GND
 * - IN 1 - GND
 * - IN 2 - GND
 * - IN 3 - GND
 * 
 * Note that all inputs must not be in the range [-0.3V, VDD+0.3V] and that 
 * voltages outside this range can harm the device!
 * 
 * The device itself can measure a single voltage difference at a time and it
 * provides a CONTINUOUS mode, which continuously repeats the measurement. This
 * class hides from the user the fact of a single measurement per time and it
 * provides its own continuous method, which internally switches the multiplexer
 * to chose all the input combinations for which the values are requested. At
 * the moment the native continuous mode is not accessible.
 * 
 * TThe ADS1115 class allows to use different gain for each of the differential
 * inputs, which can be set using the setGain(input, gain) method. It also
 * provides an automatic gain mode, which adjusts the gain automatically to try
 * to keep the measurements in the [50%, 90%] of the full range, to optimize the
 * accuracy of the results. If no gain is set, this mode is the default.
 */
class ADS1115 {
  
public:
  
  /// Enumeration representing the pin to which the ADDR pin is connected to
  enum class AddressPin {
    GND, ///< Slave address 0b1001000, 0x48
    VDD, ///< Slave address 0b1001001, 0x49
    SDA, ///< Slave address 0b1001010, 0x4A
    SCL  ///< Slave address 0b1001011, 0x4B
  };
  
  /// Defines the input of the measurement differential voltage
  enum class Input {
    AIN0_AIN1,  ///< Measured value: A0 - A1
    AIN0_AIN3,  ///< Measured value: A0 - A3
    AIN1_AIN3,  ///< Measured value: A1 - A3
    AIN2_AIN3,  ///< Measured value: A2 - A3
    AIN0_GND,   ///< Measured value: A0 - GND
    AIN1_GND,   ///< Measured value: A1 - GND
    AIN2_GND,   ///< Measured value: A2 - GND
    AIN3_GND    ///< Measured value: A3 - GND
  };
  
  /// The different available gains
  enum class Gain {
    G_2_3,   ///< Gain 2/3,  full-scale ±6.144V
    G_1,     ///< Gain   1,  full-scale ±4.096V
    G_2,     ///< Gain   2,  full-scale ±2.048V
    G_4,     ///< Gain   4,  full-scale ±1.024V
    G_8,     ///< Gain   8,  full-scale ±0.512V
    G_16,    ///< Gain  16,  full-scale ±0.256V
    AUTO
  };
  
  /// The available modes
  enum class Mode {
    CONTINUOUS,
    SINGLE_SHOT
  };
  
  /// The different available data rates
  enum class DataRate {
    DR_8_SPS,
    DR_16_SPS,
    DR_32_SPS,
    DR_64_SPS,
    DR_128_SPS,
    DR_250_SPS,
    DR_475_SPS,
    DR_860_SPS
  };
  
  /// The comparator modes
  enum class ComparatorMode {
    TRADITIONAL,
    WINDOW
  };
  
  // The number of measurements above the threshold to trigger the comparator event
  enum class ComparatorQueueSize {
    QUE_1,
    QUE_2,
    QUE_4
  };
  
  /**
   * @brief Creates a new ADS1115 instance
   * 
   * @details
   * The addr parameter controls the pin to which the ADDR pin is connected, for
   * selecting the device address. The data_rate parameter controls the sampling
   * rate the device is set. Smaller sampling rates provide more accurate
   * measurements but take more time.
   * 
   * Note that only a single instance of ADS1115 can be created for each
   * different address pin.
   * 
   * @param addr
   *    The pin at which the ADDR pin of the ADS1115 is connected
   * @param data_rate
   *    The data rate to set the device
   * @return 
   */
  static std::unique_ptr<ADS1115> factory(AddressPin addr=AddressPin::GND,
                                          DataRate data_rate=DataRate::DR_128_SPS);
  
  /// Allows other ADS1115 instances to be created for the same address
  virtual ~ADS1115();
  
  /**
   * @brief Sets the gain for a given input
   * 
   * @details
   * The ADS1115 class can use different gains for each differential input. The
   * gain values can be manually set by calling this method, to achieve the
   * best sampling of an input of known range. If GAIN::auto is passed as
   * parameter, the automatic gain mode is enabled for the specific input, which
   * will change gains automatically to try to keep the input in the [50%, 90%]
   * range of the full scale.
   * 
   * @param input
   *    The differential input to set the gain for
   * @param gain
   *    The gain
   */
  void setGain(Input input, Gain gain);
  
  /// Reads a single conversion for the given differential input
  float readConversion(Input input);
  
  /// Returns an AnalogInput for accessing the requested differential input
  std::unique_ptr<AnalogInput<float>> conversionAnalogInput(Input input);
  
  /// Add an observer which will be notified for measurements of the specified
  /// differential input
  void addConversionObserver(Input input, std::shared_ptr<Observer<float>> observer);
  
  /**
   * @brief Start continuous measurement mode
   * 
   * @details
   * This mode will continuously check for which inputs have been registered
   * observers, it will perform the measurements and will notify the observers.
   * 
   * The parameter force controls which measurements are performed. When false,
   * only the measurements for which observers have been registered will be
   * performed. This provides the optimal sampling speed. When true, all input
   * measurements will be performed, even if there are no observers. This allows
   * to trigger the hardware comparator events for all inputs, but will
   * considerably slow down the total sampling rate.
   * 
   * The parameter power_down_ms will can be used for applications that high
   * sampling rate is not required. The device will be put in power down mode
   * for this number of milliseconds every time a set of measurements is
   * performed. This can be used to limit the current consumption of the device.
   */
  void start(bool force=false, int power_down_ms=0);
  
  /// Stop the continuous measurement mode
  void stop();
  
private:
  
  ADS1115(AddressPin addr, DataRate data_rate);
  
  std::uint8_t m_addr;
  mutable std::mutex m_mutex;
  std::map<Input, Gain> m_input_gain_map;
  std::map<Input, bool> m_input_auto_gain_flag_map;
  Mode m_mode;
  DataRate m_data_rate;
  std::map<Input, EncapsulatedObservable<float>> m_input_observable_map;
  std::atomic<bool> m_observing {false};
  
}; // end of class ADS1115

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_MODULES_ADS1115_H */

