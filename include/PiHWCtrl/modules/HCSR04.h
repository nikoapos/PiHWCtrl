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
 * @file PiHWCtrl/modules/HCSR04.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_HCSR04_H
#define PIHWCTRL_HCSR04_H

#include <memory>
#include <PiHWCtrl/HWInterfaces/BinaryInput.h>
#include <PiHWCtrl/HWInterfaces/Switch.h>

namespace PiHWCtrl {

class HCSR04 {
  
public:
  
  HCSR04(std::unique_ptr<Switch> trigger, std::unique_ptr<BinaryInput> echo,
         float max_distance=4.);

  virtual ~HCSR04() = default;
  
  float readDistance();
  
private:
  
  std::unique_ptr<Switch> m_trigger;
  std::unique_ptr<BinaryInput> m_echo;
  float m_max_dist;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_HCSR04_H */

