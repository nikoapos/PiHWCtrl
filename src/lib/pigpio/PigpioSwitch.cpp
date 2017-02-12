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

PigpioSwitch::PigpioSwitch(int gpio) : m_gpio(gpio) {
  SmartPigpio::getSingleton()->reserveGpio(m_gpio);
  auto res = gpioSetMode(m_gpio, PI_OUTPUT);
  if (res == PI_BAD_GPIO) {
    throw BadGpioNumber(m_gpio);
  } else if (res == PI_BAD_MODE) {
    throw BadGpioMode(m_gpio, PI_OUTPUT);
  } else if (res != 0) {
    throw UnknownPigpioException(res);
  }
}

PigpioSwitch::PigpioSwitch(PigpioSwitch&& other) : m_gpio(other.m_gpio) {
  // Make the other object to not have control of the GPIO
  other.m_gpio = -1;
}

namespace {

void cleanup(int gpio) {
  if (gpio != -1) {
    SmartPigpio::getSingleton()->releaseGpio(gpio);
  }
}

} // end of anonymous namespace

PigpioSwitch& PigpioSwitch::operator=(PigpioSwitch&& other) {
  // We will stop using the current GPIO, so clean it up
  cleanup(m_gpio);
  // Now take over the GPIO of the other
  m_gpio = other.m_gpio;
  other.m_gpio = -1;
}

PigpioSwitch::~PigpioSwitch() {
  cleanup(m_gpio);
}

void PigpioSwitch::set(bool value) {
  if (m_gpio == -1) {
    throw BadGpioNumber(m_gpio);
  }
  unsigned int level = value ? 1 : 0;
  gpioWrite(m_gpio, level);
}

bool PigpioSwitch::isOn() const {
  if (m_gpio == -1) {
    throw BadGpioNumber(m_gpio);
  }
  return gpioRead(m_gpio);
}

} // end of namespace PiHWCtrl