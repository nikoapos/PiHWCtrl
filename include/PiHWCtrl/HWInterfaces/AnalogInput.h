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
 * @file PiHWCtrl/HWIterfaces/AnalogInput.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_ANALOGINPUT_H
#define PIHWCTRL_ANALOGINPUT_H

#include <type_traits>
#include <cstdint>

namespace PiHWCtrl {

/**
 * @class AnalogInput
 * 
 * @brief
 * Interface representing an analog input
 * 
 * @details
 * Analog input is anything that provides an arithmetic value. This interface
 * defines a method for accessing this value.
 * 
 * @tparam T
 *    The type of the value. Must be arithmetic.
 */
template <typename T>
class AnalogInput {
  static_assert(std::is_arithmetic<T>::value, "The type T of the AnalogInput must be arithmetic");
  
public:

  /// Default destructor
  virtual ~AnalogInput() = default;
  
  /// Must be implemented by the subclasses to return the value of the input
  virtual T readValue() = 0;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_ANALOGINPUT_H */

