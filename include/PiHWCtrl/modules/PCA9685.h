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

class PCA9685 {
  
public:
  
  static std::unique_ptr<PCA9685> factory(std::uint8_t address, int pwm_frequency=200);
  
  virtual ~PCA9685();
  
  void setDutyCycle(int led, float duty_cycle);
  
  float getDutyCycle(int led);
  
  std::unique_ptr<PWM> getAsPWM(int led);
  
private:
  
  PCA9685(std::uint8_t address, int pwm_frequency);
  
  std::uint8_t m_address;
  mutable std::mutex m_mutex;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_MODULES_PCA9685_H */

