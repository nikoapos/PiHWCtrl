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
 * @file PigpioSwitch.cpp
 * @author nikoapos
 */

#include <pigpio.h>
#include <PiHWCtrl/pigpio/SmartPigpio.h>
#include <PiHWCtrl/pigpio/exceptions.h>
#include <PiHWCtrl/pigpio/PigpioSwitch.h>

namespace PiHWCtrl {

PigpioSwitch::PigpioSwitch(unsigned int m_gpio) : m_gpio(m_gpio) {
  SmartPigpio::getSingleton()->reserveGpio(m_gpio);
  auto res = gpioSetMode(m_gpio, PI_OUTPUT);
  if (res == PI_BAD_GPIO) {
    throw BadGpioNumber();
  } else if (res == PI_BAD_MODE) {
    throw BadGpioMode();
  } else if (res != 0) {
    throw UnknownPigpioException();
  }
}

PigpioSwitch::~PigpioSwitch() {
  SmartPigpio::getSingleton()->releaseGpio(m_gpio);
}

void PigpioSwitch::set(bool value) {
  unsigned int level = value ? 1 : 0;
  gpioWrite(m_gpio, level);
}

bool PigpioSwitch::isOn() const {
  return gpioRead(m_gpio);
}

} // end of namespace PiHWCtrl