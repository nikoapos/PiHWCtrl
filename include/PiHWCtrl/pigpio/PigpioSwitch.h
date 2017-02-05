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
 * @file PigpioSwitch.h
 * @author nikoapos
 */

#ifndef PIGPIOSWITCH_H
#define PIGPIOSWITCH_H

#include <PiHWCtrl/HWInterfaces/Switch.h>
#include <PiHWCtrl/HWInterfaces/BinaryInput.h>

namespace PiHWCtrl {

class PigpioSwitch : public Switch, public BinaryInput {
  
public:

  PigpioSwitch(unsigned int m_gpio);
  
  virtual ~PigpioSwitch();

  void set(bool value) override;
  
  bool isOn() const override;
  
private:
  
  unsigned int m_gpio;

};

} // end of namespace PiHWCtrl

#endif /* PIGPIOSWITCH_H */

