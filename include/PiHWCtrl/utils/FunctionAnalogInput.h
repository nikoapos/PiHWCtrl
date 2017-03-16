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
 * @file PiHWCtrl/utils/FunctionAnalogInput.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_FUNCTIONANALOGINPUT_H
#define PIHWCTRL_FUNCTIONANALOGINPUT_H

#include <functional>
#include <PiHWCtrl/HWInterfaces/AnalogInput.h>

namespace PiHWCtrl {

template <typename T>
class FunctionAnalogInput : public AnalogInput<T> {
  
public:
  
  FunctionAnalogInput(std::function<T()> function) : m_function(function) {
  }

  virtual ~FunctionAnalogInput() = default;
  
  T readValue() override {
    return m_function();
  }
  
private:
  
  std::function<float()> m_function;
  
};

}

#endif /* PIHWCTRL_FUNCTIONANALOGINPUT_H */

