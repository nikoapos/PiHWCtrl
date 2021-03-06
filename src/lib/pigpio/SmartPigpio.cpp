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
 * File:   SmartPigpio.cpp
 * Author: nikoapos
 * 
 * Created on February 3, 2017, 1:58 PM
 */

#include <pigpio.h>
#include <PiHWCtrl/pigpio/exceptions.h>
#include <PiHWCtrl/pigpio/SmartPigpio.h>

namespace PiHWCtrl {

std::shared_ptr<SmartPigpio> SmartPigpio::getSingleton() {
  static std::shared_ptr<SmartPigpio> singleton = std::shared_ptr<SmartPigpio> {new SmartPigpio};
  return singleton;
}

SmartPigpio::SmartPigpio() : m_pigpio_version_number(gpioInitialise()) {
  if (m_pigpio_version_number < 0) {
    throw PigpioInitFailed(m_pigpio_version_number);
  }
  for (auto& flag : m_reserved_flags) {
    flag = false;
  }
}

SmartPigpio::~SmartPigpio() {
  gpioTerminate();
}

} // end of namespace PiHWCtrl

