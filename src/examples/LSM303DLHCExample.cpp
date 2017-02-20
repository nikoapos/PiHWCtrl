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
 * @file examples/LSM303DLHCExample.cpp
 * @author nikoapos
 */

#include <iostream>
#include <cmath>
#include <chrono> // for std::chrono_literals
#include <thread> // for std::this_thread
#include <PiHWCtrl/modules/LSM303DLHC.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

int main() {
  
  PiHWCtrl::LSM303DLHC sensor {PiHWCtrl::LSM303DLHC::MagnetometerOutputRate::R_15_HZ,
                               PiHWCtrl::LSM303DLHC::MagnetometerGain::G_1_3};
  
//  for (int i=0; i<5; ++i) {
  for (;;) {
    std::this_thread::sleep_for(100ms);
    auto v = sensor.getMagneticField();
    std::cout << "The magnetic field is " << sensor.getMagneticField() << '\n';
    auto strength = std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    std::cout << "Strength = " << strength << " Gauss\n";
    
  }
                               
//  std::int16_t x_min = 1000;
//  std::int16_t x_max = -1000;
//  std::int16_t y_min = 1000;
//  std::int16_t y_max = -1000;
//  std::int16_t z_min = 1000;
//  std::int16_t z_max = -1000;
//  for (;;) {
//    auto v = sensor.getMagneticField();
//    if (v.x < x_min) x_min = v.x;
//    if (v.x > x_max) x_max = v.x;
//    if (v.y < y_min) y_min = v.y;
//    if (v.y > y_max) y_max = v.y;
//    if (v.z < z_min) z_min = v.z;
//    if (v.z > z_max) z_max = v.z;
//    std::cout << "X " << x_min << " " << x_max << " Y " << y_min << ' ' << y_max << " Z " << z_min << ' ' << z_max << '\n';
//  }
  
}