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

/*
 * Description
 * -----------
 * 
 * Simple example of how to use the HCSR04 class. This class can control the
 * module HC-SR04, which is an ultrasonic distance measurement sensor.
 * 
 * The HC-SR04 measures distances by producing 8 ultrasound pulses at 40 kHz and
 * measuring the time required for the pulses to be reflected by an object and
 * come back. The way the HC-SR04 works is by using the two pins named Trig and
 * Echo. A short pulse of 10us at the Trig pin starts a measurement. The HC-SR04
 * sends the ultrasound pulses and waits for them to return. After the pulses
 * return, it sets the Echo pin up for as much time as it took to the pulses to
 * return. Note that if the pulses are not returned, the Echo pin will not be
 * raised at all.
 * 
 * The HCSR04 class communicates with the device via a Switch for triggering the
 * measurements and a BinaryInput for measuring the echo. Note that the class
 * handles the case of no echo raised (no pulses returned), by returning the
 * maximum distance. 
 * 
 * Hardware implementation
 * -----------------------
 * Materials:
 *   - A HC-SR04 sensor
 *   - A bidirectional logic level converter (to interface the 5V of the HC-SR04
 *     with the 3.3V of Raspberry Pi)
 * 
 * Connections:
 *   - Connect the GND of the HC-SR04 to one of the GND pins
 *   - Connect the VCC of the HC-SR04 to one of the 5V pins
 *     (Important: You must always connect first the GND and then the VCC)
 *   - Connect the GND pins of the level converter to one of the GND pins
 *   - Connect the 5V pin of the level converter to one 5V pins
 *   - Connect the 3.3V pin of the level converter to one 3.3V pins
 *   - Connect the TRIG of the HC-SR04 to one of the HI pins of the level converter
 *   - Connect the corresponding LO pin of the level converter to GPIO 21
 *   - Connect the ECHO of the HC-SR04 to one of the HI pins of the level converter
 *   - Connect the corresponding LO pin of the level converter to GPIO 26
 * 
 * Execution:
 * Run the example. You will see messages with the measured distance for 10
 * seconds.
 */

#include <iostream>  // for std::out, std::flush
#include <thread>    // for std::this_thread
#include <chrono>    // for std::chrono_literals

#include <PiHWCtrl/gpio/GpioBinaryInput.h>
#include <PiHWCtrl/gpio/GpioSwitch.h>
#include <PiHWCtrl/modules/HCSR04.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;


// This is a help method that prints on the screen the a measured value, read
// from an AnalogInput. Note that this method does not know the actual type of
// the AnalogInput, but just the interface, so it can be called with any class
// implementing the interface.
void printAnalogInput(PiHWCtrl::AnalogInput<float>& input) {
  std::cout << "The value of the analog input is: " << input.readValue() << '\n';
}


int main() {
  
  //
  // Create an object for controlling the BMP180. We will use the GPIO 21 to
  // trigger the measurements, so we create a GpioSwitch object to controll it,
  // and the GPIO 26 for measuring the echo duration, so we create a
  // GpioBinaryInput.
  //
  auto trigger = std::make_unique<PiHWCtrl::GpioSwitch>(21);
  auto echo = std::make_unique<PiHWCtrl::GpioBinaryInput>(26);
  PiHWCtrl::HCSR04 sensor {std::move(trigger), std::move(echo)};
  
  
  //
  // After you have setup your HCSR04 class, you can retrieve its measurements
  // by one of three ways, as shown below.
  //
  
  //
  // The first way to get the sensor measurements is to directly call the
  // method of the class. Note that in this case each method call will contact
  // the sensor to perform all the required measurements (no caching is
  // performed) and it will block until the measurement is done. The blocking
  // time depends on the distance measured.
  //
  std::cout << "The distance is " << sensor.readDistance() << " m\n";
  
  //
  // The second way to get the sensor measurements is to pass it as parameter
  // anywhere an AnalogInput is expected. As with the direct method calls, the
  // AnalogInput calls will trigger the measurements and will block until they
  // are finished.
  //
  printAnalogInput(sensor);
  
  //
  // The last way to get the sensor measurements is by registering observers to
  // the HCSR04 class and call its start() method. This will put the class in
  // continuous measurement mode and it will notify the observers every time a
  // measurement is finished. You can always stop the measurements by calling
  // the stop() method.
  //
  // Note that the observer methods will be called in the measurements thread,
  // so if their actions are taking too long they will block the measurements.
  // If this is the case the observer should delegate the action to a separate
  // thread.
  //
  
  // The ScreenPrinter class is the observer implementation which will be
  // registered to the HCSR04. It prints the distance on the screen, overriding
  // the same line.
  class ScreenPrinter : public PiHWCtrl::Observer<float> {
  public:
    void event(const float& value) override {
      std::cout << '\r'; // Go to the beginning of the line
      std::cout << "Distance from observer: " << value << "     " << std::flush;
    }
  };
  
  //
  // We create the instance of the observer and we add it to the HCSR04
  //
  auto screen_printer = std::make_shared<ScreenPrinter>();
  sensor.addObserver(screen_printer);
  
  //
  // We need to call the start() method so the HCSR04 will enter the continuous
  // measurement mode and it will start notifying the observers
  //
  sensor.start();
  
  //
  // We wait for 10 seconds to see the results
  //
  std::this_thread::sleep_for(10s);
  
  //
  // Optionally we an stop the sensor. This will happen anyways when the variable
  // goes out of scope.
  //
  sensor.stop();
  
  std::cout << "\n\n";
  
}