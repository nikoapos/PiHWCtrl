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
 * @file modules/PCA9685.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_MODULES_PCA9685_H
#define PIHWCTRL_MODULES_PCA9685_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <PiHWCtrl/HWInterfaces/PWM.h>

namespace PiHWCtrl {

/**
 * @class PCA9685
 * 
 * @brief
 * Class for controlling the PCA9685 PWM controller
 * 
 * @details
 * The PCA9685 has 16 PWM channels which can control directly devices which
 * drain up to 25mA current, or it can be used to control external drivers (like
 * the L293D) to drive motors or other devices which drain more current.
 * 
 */
class PCA9685 {
  
public:
  
  /**
   * @brief Creates a new PCA9685 instance
   * 
   * @details
   * The first parameter of the method is the address of the device. The PCA9685
   * allows for customization of the I2C address via 6 address pins, which can
   * be used to configure the address as the binary [1][A5][A4][A3][A2][A1][A0],
   * resulting to addresses in the range [0x40, 0x7F]. Only a single instance
   * of the PCA9685 class per address can be instantiated at any time.
   * 
   * The second parameter is the frequency of the PWMs and is common to all 16
   * channels. It is expressed in Hz and it can get any value between 20 Hz and
   * 1526 Hz.
   *  
   * @param address
   *    The I2C address of the device
   * @param pwm_frequency
   *    The PWM frequency, expressed in Hz
   * @return 
   *    The PCA9685 instance
   */
  static std::unique_ptr<PCA9685> factory(std::uint8_t address, int pwm_frequency=200);
  
  /// Allows other PCA9685 instances to be created for the same address
  virtual ~PCA9685();
  
  /**
   * @brief Sets the duty cycle of a specific channel
   * 
   * @details
   * The duty cycle can be any number in the range [0,1], where 0 means full OFF
   * and 1 means full ON.
   * 
   * @param channel
   *    The channel to set the PWM duty cycle for
   * @param duty_cycle
   *    The duty cycle in the range [0,1]
   */
  void setDutyCycle(int channel, float duty_cycle);
  
  /// Returns the duty cycle for the specified channel
  float getDutyCycle(int channel);
  
  /// Returns a PWM object which can be used to control the given channel
  std::unique_ptr<PWM> getAsPWM(int channel);
  
private:
  
  PCA9685(std::uint8_t address, int pwm_frequency);
  
  std::uint8_t m_address;
  mutable std::mutex m_mutex;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_MODULES_PCA9685_H */

