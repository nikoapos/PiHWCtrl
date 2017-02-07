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
 * @file PiHWCtrl/HWInterfaces/BinaryInput.h
 * @author nikoapos
 */

#ifndef BINARYINPUT_H
#define BINARYINPUT_H

namespace PiHWCtrl {

/**
 * @class BinaryInput
 * 
 * @brief
 * Interface representing a binary input
 * 
 * @details
 * Binary input is anything that can have two states, ON and OFF. This interface
 * defines methods for accessing this state.
 */
class BinaryInput {
  
public:
  
  /// Default destructor
  virtual ~BinaryInput() = default;

  /// Must be implemented by the subclasses to return true if the state is ON
  /// and false when the state is OFF
  virtual bool isOn() const = 0;
  
  /// Implemented to return the opposite of isOn()
  virtual bool isOff() const final;
  
};

} // end of namespace PiHWCtrl

#endif /* BINARYINPUT_H */

