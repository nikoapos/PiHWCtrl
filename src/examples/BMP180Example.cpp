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
 * @file examples/BMP180Example.cpp
 * @author nikoapos
 */

/*
 * Description
 * -----------
 * 
 * Simple example of how to use the BMP180 class. This class can control the
 * module BMP180, which is a pressure and temperature sensor controlled using
 * the I2C protocol.
 * 
 * The BMP180 class provides the measured pressure and temperature values and
 * also provides the altitude, which is computed based on the sea level
 * pressure, which can either be given by the user or it can be calibrated using
 * a known altitude.
 * 
 * Hardware implementation
 * -----------------------
 * Materials:
 *   - A BMP180 breakout
 * 
 * Connections:
 *   - Connect the GND of the BMP180 to one of the GND pins
 *   - Connect the VIN of the BMP180 to one of the 3.3V pins
 *   - Connect the SCL of the BMP180 to the SCL pin (pin 5 / GPIO 3)
 *   - Connect the SDA of the BMP180 to the SDA pin (pin 3 / GPIO 2)
 * 
 * Execution:
 * Run the example. You will see messages with the current temperature, pressure
 * and altitude for 10 seconds.
 */

#include <iostream>   // for std::out, std::flush
#include <iomanip>    // for std::left, std::setw
#include <thread>     // for std::this_thread
#include <chrono>     // for std::chrono_literals
#include <functional> // for std::function, std::bind

#include <PiHWCtrl/modules/BMP180.h> // for PiHWCtrl::BMP180

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

// For _1, _2, _3... parameters of the std::bind
using namespace std::placeholders;


// This is a help method that prints on the screen the a measured value, read
// from an AnalogInput. Note that this method does not know the actual type of
// the AnalogInput, but just the interface, so it can be called with any class
// implementing the interface.
template <typename T>
void print(const std::string& name, PiHWCtrl::AnalogInput<T>& analog_input) {
  std::cout << std::right << std::setw(20) << name << ": ";
  std::cout << analog_input.readValue() << '\n';
}


int main() {
  
  //
  // Create an object for controlling the BMP180. The first parameter of the
  // factory is the pressure sampling accuracy, which can be one of the
  // following:
  //        ULTRA_LOW_POWER : Conversion time  4.5ms. RMS 0.06hPa - 0.5m
  //               STANDARD : Conversion time  7.5ms, RMS 0.05hPa - 0.4m (default)
  //        HIGH_RESOLUTION : Conversion time 13.5ms, RMS 0.04hPa - 0.3m
  //  ULTRA_HIGH_RESOLUTION : Conversion time 25.5ms, RMS 0.03hPa - 0.25m
  //
  // The second parameter is the value of the sea level pressure, which will be
  // used for computing the altitude.
  //
  // Note there can be a single instance of the BMP180 class, as it represents
  // the actual hardware. For this reason the factory returns a unique_ptr to
  // the instance. If you want to share it between classes, you can store it in
  // a shared_ptr.
  //
  auto sensor = PiHWCtrl::BMP180::factory(PiHWCtrl::BMP180::PressureMode::ULTRA_HIGH_RESOLUTION);
  
  //
  // If you know your exact altitude, you can use it to calibrate the sea level
  // pressure used for the altitude measurements. The BMP180 class provides a
  // method for retrieving the computed sea level pressure, so you can even use
  // it for implementing a weather station.
  //
  sensor->calibrateSeaLevelPressure(486);
  auto sea_pres = sensor->getSeaLevelPressure();
  std::cout << "Assuming you are at 486 m altitude, the sea level pressure is ";
  std::cout << sea_pres << '\n';
  
  //
  // You can always manually update the sea level pressure after you create the
  // BMP180 instance. This is useful, for example, if you periodically check a
  // weather station to get an up to date value.
  //
  sensor->setSeaLevelPressure(1030);
  
  //
  // After you have setup your BMP180 class, you can retrieve its measurements
  // by one of three ways, as shown below.
  //
  
  //
  // The first way to get the sensor measurements is to directly call the
  // methods of the class. Note that in this case each method call will contact
  // the sensor to perform all the required measurements (no caching is
  // performed) and it will block until the measurement is done. The blocking
  // time depends on the method and the pressure sampling accuracy selected.
  //
  std::cout << "\nGetting the information using direct function calls:\n";
  auto raw_temp = sensor->readRawTemperature();
  auto temp = sensor->readTemperature();
  auto raw_pres = sensor->readRawPressure();
  auto pres = sensor->readPressure();
  auto alt = sensor->readAltitude();
  std::cout << "     Raw temperature: " << raw_temp << "\n";
  std::cout << "         Temperature: " << temp << " C\n";
  std::cout << "        Raw pressure: " << raw_pres << "\n";
  std::cout << "            Pressure: " << pres << " hPa\n";
  std::cout << "            Altitude: " << alt << " m\n";
  
  //
  // The second way to get the sensor measurements is by using the methods that
  // return AnalogInput implementations. The objects returned by these methods
  // are implementing the AnalogInput interface and can be used anywhere an
  // AnalogInput is expected. As with the direct method calls, the AnalogInputs
  // will trigger the measurements and will block until they are finished.
  //
  std::cout << "\nGetting the information using the AnalogInputs:\n";
  auto raw_temp_ai = sensor->rawTemperatureAnalogInput();
  auto temp_ai = sensor->temperatureAnalogInput();
  auto raw_pres_ai = sensor->rawPressureAnalogInput();
  auto pres_ai = sensor->pressureAnalogInput();
  auto alt_ai = sensor->altitudeAnalogInput();
  print("Raw temperature", *raw_temp_ai);
  print("Temperature", *temp_ai);
  print("Raw pressure", *raw_pres_ai);
  print("Pressure", *pres_ai);
  print("Altitude", *alt_ai);
  
  //
  // The last way to get the sensor measurements is by registering observers to
  // the BMP180 class and call its start() method. This will put the class in
  // continuous measurement mode and it will notify the observers every time a
  // measurement is finished. You can always stop the measurements by calling
  // the stop() method.
  //
  // Note that the observer methods will be called in the measurements thread,
  // so if their actions are taking too long they will block the measurements.
  // If this is the case the observer should delegate the action to a separate
  // thread.
  //
  std::cout << "\nGetting the information using observers:\n";
  
  // This is a helper class that is used to print the messages on the screen.
  // Each time the print() method is called it overrides the current line with
  // the new values. For simplicity we only show the temperature and the
  // altitude.
  class ScreenPrinter {
  public:
    void print() {
      std::cout << '\r'; // Go to the beginning of the line
      std::cout << "  Temperature: " << std::left << std::setw(6) << m_temp;
      std::cout << "Altitude: " << std::left << std::setw(12) << m_alt << std::flush;
    }
    void setTemperature(float temp) {
      m_temp = temp;
      print();
    }
    void setAltitude(float alt) {
      m_alt = alt;
      print();
    }
  private:
    float m_temp = 0;
    float m_alt = 0;
  };
  
  // We create an instance of the ScreenPrinter, which will be updated by the
  // observers
  ScreenPrinter printer {};
  
  // The PrinterUpdater class is the observer implementation which will be
  // registered to the BMP180. Internally it keeps a function to which it will
  // redirect the value when an event occurs.
  class PrinterUpdater : public PiHWCtrl::Observer<float> {
  public:
    PrinterUpdater(std::function<void(float)> func) : m_func(func) { }
    void event(const float& value) override {
      m_func(value);
    }
  private:
    std::function<void(float)> m_func;
  };
  
  //
  // We create two instances of the PrinterUpdater and we add them as observers
  // to the BMP180. One that redirects the calls to the setTemperature() method
  // of the ScreenPrinter and one that redirects the calls to the setAltitude().
  //
  auto set_temp_func = std::bind(&ScreenPrinter::setTemperature, &printer, _1);
  auto temp_observer = std::make_shared<PrinterUpdater>(set_temp_func);
  sensor->addTemperatureObserver(temp_observer);
  
  auto set_alt_func = std::bind(&ScreenPrinter::setAltitude, &printer, _1);
  auto alt_observer = std::make_shared<PrinterUpdater>(set_alt_func);
  sensor->addAltitudeObserver(alt_observer);
  
  //
  // We need to call the start() method so the BMP180 will enter the continuous
  // measurement mode and it will start notifying the observers
  //
  sensor->start();
  
  //
  // We wait for 10 seconds to see the results
  //
  std::this_thread::sleep_for(10s);
  
  //
  // Optionally we an stop the sensor. This will happen anyways when the variable
  // goes out of scope.
  //
  sensor->stop();
  
  std::cout << "\n\n";
  
}