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
#include <PiHWCtrl/utils/EventGenerator.h>
#include <PiHWCtrl/HWInterfaces/exceptions.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

namespace PiHWCtrl {

HCSR04::HCSR04(std::unique_ptr<Switch> trigger, std::unique_ptr<BinaryInput> echo,
               float max_distance, float sound_speed)
        : m_trigger(std::move(trigger)), m_echo(std::move(echo)),
          m_max_dist(max_distance), m_sound_speed(sound_speed) {
  m_trigger->turnOff();
}

HCSR04::~HCSR04() {
  // Stop any threads generating events for this HCSR04
  stop();
}

float HCSR04::readDistance() {
  
  std::lock_guard<std::mutex> lock {m_mutex};
  
  // Set the trigger to on for 10us to start the measurement
  m_trigger->turnOn();
  std::this_thread::sleep_for(10us);
  m_trigger->turnOff();
  
  // Wait for the measurement to happen. It it takes more than what is required
  // for the current maximum distance we return the maximum distance directly.
  int max_time = 2E3 * m_max_dist / m_sound_speed;
  auto t1 = std::chrono::high_resolution_clock::now();
  for (; m_echo->isOff() ;) {
    auto t2 = std::chrono::high_resolution_clock::now();
    if (t2 > t1 + std::chrono::milliseconds(max_time)) {
      return m_max_dist;
    }
  }
  
  // The echo just turned on. Count how long it will stay up.
  t1 = std::chrono::high_resolution_clock::now();
  auto t2 = t1;
  for(; m_echo->isOn() ;) {
    t2 = std::chrono::high_resolution_clock::now();
  }
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  // Convert to meters : x(m) = (t(us) * sound_speed(m/s) / 2) / 10^6
  float dist = time * m_sound_speed / 2e6;
  // If the measured distance is greater than the maximum we return the maximum
  return std::min(dist, m_max_dist);
}

float HCSR04::readValue() {
  return readDistance();
}

void HCSR04::start(unsigned int sleep_ms) {
  if (m_observing) {
    throw Exception() << "HCSR04 already started";
  }
  m_observing = true;
  auto event_func = [this]() {
    return readDistance();
  };
  auto notify_func = [this](float value) {
    notifyObservers(value);
  };
  startEventGenerator<float>(event_func, notify_func, m_observing, sleep_ms);
}

void HCSR04::stop() {
  if (m_observing) {
    // This will trigger the EventGenerator thread to stop
    m_observing = false;
    // We have to wait until the thread signals that it stopped
    while (!m_observing) {
    }
    // Now we can set again the flag to false
    m_observing = false;
  }
}

} // end of namespace PiHWCtrl