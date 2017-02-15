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
 * @file examples/HCSR04Example.cpp
 * @author nikoapos
 */

#include <iostream>
#include <PiHWCtrl/pigpio/PigpioBinaryInput.h>
#include <PiHWCtrl/pigpio/PigpioSwitch.h>
#include <PiHWCtrl/modules/HCSR04.h>

int main() {
  
  auto trigger = std::make_unique<PiHWCtrl::PigpioSwitch>(21);
  auto echo = std::make_unique<PiHWCtrl::PigpioBinaryInput>(26);
  
  PiHWCtrl::HCSR04 sensor {std::move(trigger), std::move(echo)};
  
  std::cout << "The distance is " << sensor.readDistance() << " m\n";
  
}