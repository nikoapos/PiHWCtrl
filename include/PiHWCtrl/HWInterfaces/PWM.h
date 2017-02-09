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
 * @file PiHWCtrl/HWInterfaces/PWM.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_PWM_H
#define PIHWCTRL_PWM_H

namespace PiHWCtrl {

/**
 * @class PWM
 * 
 * @brief
 * Interface representing a PWM (Pulse Width Modulation) controller
 * 
 * @details
 * The interface abstracts any details (like frequency, scale, etc) and it uses
 * a float number in the range [0, 1] to indicate the duty cycle. Any other
 * details should be customized ny the specific implementations per-case.
 */
class PWM {
  
public:
  
  /// Default destructor
  virtual ~PWM() = default;
  
  /// Must be implemented by the subclasses to set the duty cycle with a number
  /// in the range [0, 1] where 0 means off and 1 means fully on.
  virtual void setDutyCycle(float duty_cycle) = 0;
  
  /// Must be implemented by the subclasses to return the a number in the range
  /// [0, 1] indicating the current duty cycle, where 0 means off and 1 means
  /// fully on.
  virtual float getDutyCycle() = 0;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_PWM_H */

