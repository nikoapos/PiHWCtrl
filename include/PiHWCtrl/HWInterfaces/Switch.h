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
 * @file PiHWCtrl/HWInterfaces/Switch.h
 * @author nikoapos
 */

#ifndef SWITCH_H
#define SWITCH_H

namespace PiHWCtrl {

/**
 * @class Switch
 * 
 * @brief
 * Interface representing a switch
 * 
 * @details
 * A switch is anything that can be turned ON and OFF. This interface defines
 * methods for setting these two states.
 */
class Switch {
  
public:
  
  /// Default destructor
  virtual ~Switch() = default;
  
  /// Must be implemented by the subclasses to set the switch state to ON when
  /// it is called with true and to OFF when it is called with false.
  virtual void set(bool value) = 0;
  
  /// Turns the switch ON
  virtual void turnOn() final;
  
  /// Turns the switch OFF
  virtual void turnOff() final;
  
};

} // end of namespace PiHWCtrl

#endif /* SWITCH_H */

