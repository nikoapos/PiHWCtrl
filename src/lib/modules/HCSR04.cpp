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
 * @file modules/HCSR04.cpp
 * @author nikoapos
 */

#include <chrono> // for std::chrono_literals and high_resolution_clock
#include <thread> // for std::this_thread
#include <algorithm> // for std::min
#include <PiHWCtrl/modules/HCSR04.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

namespace PiHWCtrl {

HCSR04::HCSR04(std::unique_ptr<Switch> trigger, std::unique_ptr<BinaryInput> echo,
               float max_distance)
        : m_trigger(std::move(trigger)), m_echo(std::move(echo)), m_max_dist(max_distance) {
  m_trigger->turnOff();
}

float HCSR04::readDistance() {
  // Set the trigger to on for 10us to start the measurement
  m_trigger->turnOn();
  std::this_thread::sleep_for(10us);
  m_trigger->turnOff();
  
  // Wait for the measurement to happen. If this takes more than 100 milliseconds
  // (equivalent to 17.24 meters, so way out of the range of the sensor) we return
  // the maximum distance.
  auto t1 = std::chrono::high_resolution_clock::now();
  for (; m_echo->isOff() ;) {
    auto t2 = std::chrono::high_resolution_clock::now();
    if (t2 > t1 + 100ms) {
      return m_max_dist;
    }
  }
  
  // The echo just turned on. Count how long it will stay up.
  t1 = std::chrono::high_resolution_clock::now();
  auto t2 = t1;
  for(; m_echo->isOn() ;) {
    t2 = std::chrono::high_resolution_clock::now();
    if (t2 > t1 + 100ms) {
      return m_max_dist;
    }
  }
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  // Convert to meters
  float dist = time / 5800.;
  return std::min(dist, m_max_dist);
}

} // end of namespace PiHWCtrl