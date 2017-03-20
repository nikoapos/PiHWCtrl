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
 * @file ADS1115Example.cpp
 * @author nikoapos
 */

#include <iostream>   // for std::out
#include <iomanip>
#include <chrono> // for std::chrono_literals
#include <thread>
#include <functional> // for std::function, std::bind

#include <PiHWCtrl/modules/ADS1115.h> // for PiHWCtrl::ADS1115

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
  std::cout << std::right << std::setw(10) << name << ": ";
  std::cout << analog_input.readValue() << '\n';
}

int main() {
  
  auto sensor = PiHWCtrl::ADS1115::factory(PiHWCtrl::ADS1115::AddressPin::GND,
                                           PiHWCtrl::ADS1115::DataRate::DR_32_SPS);
  
  std::cout << "\nGetting the measurements using direct function calls:\n";
  auto in0 = sensor->readConversion(PiHWCtrl::ADS1115::Input::AIN0_GND);
  std::cout << "       IN0: " << in0 << " V\n";
  auto in2 = sensor->readConversion(PiHWCtrl::ADS1115::Input::AIN2_AIN3);
  std::cout << "   IN2-IN3: " << in2 << " V\n";
  
  std::cout << "\nGetting the information using the AnalogInputs:\n";
  auto in0_ai = sensor->conversionAnalogInput(PiHWCtrl::ADS1115::Input::AIN0_GND);
  print("IN0", *in0_ai);
  auto in2_in3_ai = sensor->conversionAnalogInput(PiHWCtrl::ADS1115::Input::AIN2_AIN3);
  print("IN2-IN3", *in2_in3_ai);
  
  std::cout << "\nGetting the information using observers:\n";
  
  class ScreenPrinter {
  public:
    void print() {
      std::cout << '\r'; // Go to the beginning of the line
      std::cout << "  IN0: " << std::left << std::setw(15) << m_in0;
      std::cout << "IN2-IN3: " << std::left << std::setw(15) << m_in2_in3 << std::flush;
    }
    void setIn0(float in0) {
      m_in0 = in0;
      print();
    }
    void setIn2In3(float in2_in3) {
      m_in2_in3 = in2_in3;
      print();
    }
  private:
    float m_in0 = 0;
    float m_in2_in3 = 0;
  };
  
  // We create an instance of the ScreenPrinter, which will be updated by the
  // observers
  ScreenPrinter printer {};
  
  // The PrinterUpdater class is the observer implementation which will be
  // registered to the ADS1115. Internally it keeps a function to which it will
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
  
  auto set_in0_func = std::bind(&ScreenPrinter::setIn0, &printer, _1);
  auto in0_observer = std::make_shared<PrinterUpdater>(set_in0_func);
  sensor->addConversionObserver(PiHWCtrl::ADS1115::Input::AIN0_GND, in0_observer);
  
  auto set_in2_in3_func = std::bind(&ScreenPrinter::setIn2In3, &printer, _1);
  auto in2_in3_observer = std::make_shared<PrinterUpdater>(set_in2_in3_func);
  sensor->addConversionObserver(PiHWCtrl::ADS1115::Input::AIN2_AIN3, in2_in3_observer);
  
  sensor->start();
  
  std::this_thread::sleep_for(10s);
  
  //
  // Optionally we an stop the sensor. This will happen anyways when the variable
  // goes out of scope.
  //
  sensor->stop();
  
  std::cout << "\n\n";
  
}