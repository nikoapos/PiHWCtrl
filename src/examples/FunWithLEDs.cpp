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
 * @file FunWithLEDs.cpp
 * @author nikoapos
 */

#include <vector>
#include <chrono>
#include <thread>
#include <PiHWCtrl/pigpio/PigpioBinaryInput.h>
#include <PiHWCtrl/pigpio/PigpioSwitch.h>

int main() {
  
  PiHWCtrl::PigpioBinaryInput on_off {18};
  
  std::vector<PiHWCtrl::PigpioSwitch> leds {};
  leds.emplace_back(16);
  leds.emplace_back(5);
  leds.emplace_back(25);
  leds.emplace_back(22);
  leds.emplace_back(4);
  
  unsigned int i = 0;
  int direction = 1;
  
  for (;;) {
    // First turn off all the LEDs
    for (auto& led : leds) {
      led.turnOff();
    }
    
    // Move i to the next LED
    i += direction;
    if (i == -1 || i == leds.size()) {
      direction = -1 * direction;
      i += 2 * direction;
    }
    
    // If the input is on, light the LED at position i
    if (on_off.isOn()) {
      leds[i].turnOn();
    }
    
    // Sleep for 100 ms
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
  }
  
}