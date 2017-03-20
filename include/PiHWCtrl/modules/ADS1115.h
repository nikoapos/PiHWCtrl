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
    G_16     ///< Gain  16,  full-scale ±0.256V
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
  
  enum class ComparatorQueueSize {
    QUE_1,
    QUE_2,
    QUE_4
  };
  
  static std::unique_ptr<ADS1115> factory(AddressPin addr=AddressPin::GND,
                                          Gain gain=Gain::G_2,
                                          DataRate data_rate=DataRate::DR_128_SPS);
  
  virtual ~ADS1115();
  
  float readConversion(Input input);
  
  std::unique_ptr<AnalogInput<float>> conversionAnalogInput(Input input);
  
  void addConversionObserver(Input input, std::shared_ptr<Observer<float>> observer);
  
  void start();
  
  void stop();
  
private:
  
  ADS1115(AddressPin addr, Gain gain, DataRate data_rate);
  
  std::uint8_t m_addr;
  mutable std::mutex m_mutex;
  Gain m_gain;
  Mode m_mode;
  DataRate m_data_rate;
  std::map<Input, EncapsulatedObservable<float>> m_observable_map;
  std::atomic<bool> m_observing {false};
  
}; // end of class ADS1115

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_MODULES_ADS1115_H */

