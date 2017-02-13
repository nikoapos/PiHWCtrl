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
 * @file modules/BMP180.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_BMP180_H
#define PIHWCTRL_BMP180_H

#include <cstdint>

namespace PiHWCtrl {

class BMP180 {
  
public:
  
  BMP180();
  
  virtual ~BMP180() = default;
  
  float getTemperature();
  
private:
  
  std::int16_t m_ac1;
  std::int16_t m_ac2;
  std::int16_t m_ac3;
  std::uint16_t m_ac4;
  std::uint16_t m_ac5;
  std::uint16_t m_ac6;
  std::int16_t m_b1;
  std::int16_t m_b2;
  std::int16_t m_mb;
  std::int16_t m_mc;
  std::int16_t m_md;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_BMP180_H */

