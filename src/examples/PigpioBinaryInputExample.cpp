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
 * Description
 * -----------
 * 
 * Simple example of how to use the PigpioBinaryInput class. This class
 * implements the PiHWCtrl::BinaryInput interface and can be used to read
 * binary input from one of the GPIO pins accessible via the 40 pin interface
 * (GPIOs 2-28). It interprets the input as following:
 * 
 *   - ON: 3.3 Volt connected to the pin
 *   - OFF: GND connected to the pin or the pin is open circuited
 * 
 * Under the hood, this class uses the pigpio library, so any program using it
 * must be executed with root privileges (sudo).
 * 
 * Hardware implementation
 * -----------------------
 * Materials:
 *   - A switch
 * 
 * Connections:
 *   - Connect the GPIO-21 pin to the one side of the switch
 *   - Connect a 3.3 Volt pin to the other side of the switch
 * 
 * Execution:
 * Turn the switch off and run the example. Then turn the switch on and re-run
 * it. You should see terminal messages indicating the possition of the switch.
 * 
 * @file PigpioBinaryInputExample.cpp
 * @author nikoapos
 */

#include <iostream> // for std::cout
#include <PiHWCtrl/pigpio/PigpioBinaryInput.h> // for PigpioBinaryInput

int main() {
  
  // Create an object for getting binary input from the GPIO 21. This object
  // implements the PhHWCtrl::BinaryInput interface, so it can be used with any
  // function expecting the interface.
  PiHWCtrl::PigpioBinaryInput input {21};
  
  // For checking if the input pin is on or off you can use the isOn() and
  // isOff() methods, which return a boolean.
  if (input.isOn()) {
    std::cout << "GPIO 21 input is ON\n";
  }
  if (input.isOff()) {
    std::cout << "GPIO 21 input is OFF\n";
  }
  
  // For more details (like the possible exceptions thrown) have a look at the
  // API documentation of the class!
  
}