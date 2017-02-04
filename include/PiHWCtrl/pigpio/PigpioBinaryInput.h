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
 * File:   PigpioBinaryInput.h
 * Author: nikoapos
 *
 * Created on February 3, 2017, 11:16 PM
 */

#include <PiHWCtrl/HWInterfaces/BinaryInput.h>

#ifndef PIGPIOBINARYINPUT_H
#define PIGPIOBINARYINPUT_H

namespace PiHWCtrl {

class PigpioBinaryInput : public BinaryInput {
  
public:
  
  PigpioBinaryInput(unsigned int gpio);
  
  virtual ~PigpioBinaryInput();
  
  bool isOn() override;
  
private:
  
  unsigned int m_gpio;

};

} // end of namespace PiHWCtrl

#endif /* PIGPIOBINARYINPUT_H */

