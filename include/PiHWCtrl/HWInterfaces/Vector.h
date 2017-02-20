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
 * @file PiHWCtrl/HWInterfaces/Vector.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_VECTOR_H
#define PIHWCTRL_VECTOR_H

#include <ostream>

namespace PiHWCtrl {

class Vector {
  
public:
  
  Vector(float x, float y, float z) : x(x), y(y), z(z) {
  }
  
  float x;
  float y;
  float z;
  
};

std::ostream& operator<<(std::ostream& out, const Vector& v) {
  out << '(' << v.x << '\t' << v.y << '\t' << v.z << ')';
  return out;
}

}

#endif /* PIHWCTRL_VECTOR_H */

