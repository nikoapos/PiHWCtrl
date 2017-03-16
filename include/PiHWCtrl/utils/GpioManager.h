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
 * @file utils/GpioManager.h
 * @author nikoapos
 */

#ifndef PIHECTRL_GPIOMANAGER_H
#define PIHECTRL_GPIOMANAGER_H

#include <array>
#include <memory>

namespace PiHWCtrl {

class GpioManager {
  
public:
  
  class GpioReservation;
  
  static std::shared_ptr<GpioManager> getSingleton();
  
  virtual ~GpioManager() = default;
  
  std::unique_ptr<GpioReservation> reserveGpio(int gpio);
  
private:
  
  GpioManager() = default;
  
  std::array<bool, 29> m_reserved_flags;
  
};

class GpioManager::GpioReservation {
  
public:

  GpioReservation(bool& flag) : m_flag(flag) { }

  // We do not want to allow to copy or move the reservation. Its lifetime should
  // be controlled by the unique_ptr returned by the reserveGpio().
  GpioReservation(const GpioReservation&) = delete;
  GpioReservation& operator=(const GpioReservation&) = delete;
  GpioReservation(GpioReservation&&) = delete;
  GpioReservation& operator=(GpioReservation&&) = delete;

  ~GpioReservation() {
    m_flag.get() = false;
  }

private:

  std::reference_wrapper<bool> m_flag;

};

} // end of namespace PiHWCtrl

#endif /* PIHECTRL_GPIOMANAGER_H */

