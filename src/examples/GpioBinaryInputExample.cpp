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
 * @file examples/PigpioBinaryInputExample.cpp
 * @author nikoapos
 */

/* 
 * Description
 * -----------
 * 
 * Simple example of how to use the GpioBinaryInput class. This class
 * implements the PiHWCtrl::BinaryInput interface and can be used to read
 * binary input from one of the GPIO pins accessible via the 40 pin interface
 * (GPIOs 2-28). It interprets the input as following:
 * 
 * - ON: 3.3 Volt connected to the pin
 * - OFF: GND connected to the pin or the pin is open circuited
 * 
 * Hardware implementation
 * -----------------------
 * Materials:
 *   - A switch
 *   - A 10k resistor (used as pull-down)
 * 
 * Connections:
 *   - Connect the GPIO-21 pin to the one side of the switch
 *   - Connect a 3.3V pin to the other side of the switch
 *   - Connect the one side of the resistor to the GPIO-21
 *   - Connect the other side of the resistor to a GND pin
 * 
 * Execution:
 * Run the example. A message will appear indicating if the switch is ON or OFF.
 * The program will continue to run for 10 seconds. during this time try to turn
 * on and off the switch. You should see messages indicating your actions.
 */

#include <iostream> // for std::cout
#include <thread> // for std::this_thread
#include <chrono> // for std::chrono_literals
#include <PiHWCtrl/gpio/GpioBinaryInput.h> // for GpioBinaryInput
#include <PiHWCtrl/controls/StateChangeFilter.h> // For StateChangeFilter

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

int main() {
  
  // Create an object for getting binary input from the GPIO 21. This object
  // implements the PiHWCtrl::BinaryInput interface, so it can be used with any
  // function expecting the interface.
  PiHWCtrl::GpioBinaryInput input {21};
  
  //
  // Getting the state of the pin using method calls
  //
  
  // For checking if the input pin is on or off you can use the isOn() and
  // isOff() methods, which return a boolean.
  if (input.isOn()) {
    std::cout << "GPIO 21 input is ON\n";
  }
  if (input.isOff()) {
    std::cout << "GPIO 21 input is OFF\n";
  }
  
  //
  // Getting the state of the pin using events
  //
  
  // The GpioBinaryInput class implements the Observable<bool> interface, which
  // means you can add observers that will be notified for the state of the pin.
  // The following class is an implementation of such an observer, which will
  // print on the screen the message ON or OFF.
  class ScreenPrinter : public PiHWCtrl::Observer<bool> {
  public:
    void event(const bool& value) override {
      std::cout << (value ? "ON" : "OFF") << '\n';
    }
  };
  
  // Create an instance of the observer class, which will be used for printing
  // the messages. You need to pass a shared pointer to the GpioBinaryInput, so
  // the best way to create it is by using the make_shared() method.
  auto screen_printer = std::make_shared<ScreenPrinter>();
  
  // The GpioBinaryInput will generate events with its state continuously. That
  // means that if we add the screen_printer observer directly we will get
  // repetitive messages on the screen. To solve this problem you can wrap the
  // observer with the StateChangeFilter decorator. This decorator will filter
  // out any events for which the state is not changed and will forward only
  // the events that the switch turns from OFF to ON and from ON to OFF.
  auto wrapped_observer = std::make_shared<PiHWCtrl::StateChangeFilter<bool>>(screen_printer);
  
  // Add the observer to the input, so it will be notified for its events
  input.addObserver(wrapped_observer);
  
  // To have the GpioBinaryInput generating events you have to start it by
  // calling the start() method. This method gets a single argument, which is
  // the frequency (in milliseconds) in which the events will be created. Here
  // we want to get an event every 10 ms.
  input.start(10);
  
  // Now we can sleep for 10 seconds before allowing the program to exit, so we
  // can turn the switch on and off and see the messages on the screen.
  std::this_thread::sleep_for(10s);
  
}