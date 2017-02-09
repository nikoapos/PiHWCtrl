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
 * @file examples/PigpioPWMExample.cpp
 * @author nikoapos
 */

/*
 * Description
 * -----------
 * 
 * Simple example of how to use the PigpioPWM class. This class implements the
 * PiHWCtrl::PWM interface and can be used to send PWM signals to any of the
 * GPIO pins accessible via the 40 pin interface (GPIOs 2-28). Note that this
 * signal is NOT hardware PWM, but driven via DMA. For real hardware PWM see the
 * PigpioHardwarePWM class.
 * 
 * Under the hood, this class uses the pigpio library, so any program using it
 * must be executed with root privileges (sudo).
 * 
 * For more advanced functionality (like setting the PWM frequency) see the API
 * documentation of the class itself.
 * 
 * Hardware implementation
 * -----------------------
 * Materials:
 *   - A LED (with forward voltage less than 3.3 Volt)
 *   - A 330 ohm resistor (if you want to properly calculate the value see
 *     here: http://www.evilmadscientist.com/2012/resistors-for-leds)
 * 
 * Connections:
 *   - Connect the GPIO-21 pin to the anode of the LED (longer leg)
 *   - Connect the cathode of the LED (shorter leg) to the one side of the
 *     resistor
 *   - Connect the other side of the resistor to the ground (GND)
 * 
 * Execution:
 * Run the example. The LED should gradually change from fully off to full on
 * and back indefinitely.
 */

#include <thread> // for std::this_thread
#include <chrono> // for std::chronoe_literals
#include <PiHWCtrl/pigpio/PigpioPWM.h> // for PigpioPWM

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

int main() {
  
  // Create an object for sending the PWM signal to the GPIO 21. This object
  // implements the PiHWCtrl::PWM interface, so it can be used with any function
  // expecting the interface.
  PiHWCtrl::PigpioPWM pwm {21};
  
  // We will change the brightness of the LED in steps of 1%
  float step = 0.01;
  
  // Start an infinite loop
  for (;;) {
    
    // We compute the new duty cycle by getting the current value (using the
    // getDutyCycle() method) and adding the step
    float new_duty_cycle = pwm.getDutyCycle() + step;
    
    // If the new duty cycle is outside the range [0%, 100%] we change the
    // direction
    if (new_duty_cycle < 0 || new_duty_cycle > 1) {
      step *= -1;
      new_duty_cycle += 2 * step;
    }
    
    // Set the new duty cycle of the PWM
    pwm.setDutyCycle(new_duty_cycle);
    
    // Wait for 10 milliseconds
    std::this_thread::sleep_for(10ms);
  }
  
}