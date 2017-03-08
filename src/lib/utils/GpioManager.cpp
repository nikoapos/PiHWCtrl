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
 * @file utils/GpioManager.cpp
 * @author nikoapos
 */

#include <PiHWCtrl/HWInterfaces/exceptions.h>
#include <PiHWCtrl/utils/GpioManager.h>

namespace PiHWCtrl {

std::shared_ptr<GpioManager> GpioManager::getSingleton() {
  static std::shared_ptr<GpioManager> singleton = std::shared_ptr<GpioManager>(new GpioManager{});
  return singleton;
}

auto GpioManager::reserveGpio(int gpio) -> std::unique_ptr<GpioReservation> {
  if (gpio < 2 || gpio > 28) {
    throw BadGpioNumber(gpio);
  }
  if (m_reserved_flags[gpio]) {
    throw GpioAlreadyReserved(gpio);
  }
  m_reserved_flags[gpio] = true;
  return std::make_unique<GpioReservation>(m_reserved_flags[gpio]);
}

} // end of namespace PiHWCtrl