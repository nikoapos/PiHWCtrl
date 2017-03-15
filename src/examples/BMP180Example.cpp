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

#include <iostream>
#include <thread> // for std::this_thread
#include <chrono> // for std::chrono_literals
#include <PiHWCtrl/modules/BMP180.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

void print(std::uint16_t raw_temp, float temp, std::uint16_t raw_pres,
           float pres, float alt) {
  std::cout << "    Raw temperature: " << raw_temp << "\n";
  std::cout << "        Temperature: " << temp << " C\n";
  std::cout << "       Raw pressure: " << raw_pres << "\n";
  std::cout << "           Pressure: " << pres << " hPa\n";
  std::cout << "           Altitude: " << alt << " m\n";
}

  
template <typename T, typename Printer>
struct PrinterUpdater : public PiHWCtrl::Observer<T> {
  PrinterUpdater(T& var, Printer& printer) : var(var), printer(printer) {
  }
  void event(const T& value) override {
    var = value;
    printer.reprint();
  }
  T& var;
  Printer& printer;
};

int main() {
  
  auto sensor = PiHWCtrl::BMP180::factory(PiHWCtrl::BMP180::PressureMode::ULTRA_HIGH_RESOLUTION);
  
  std::cout << "Getting the information using direct function calls:\n";
  auto raw_temp = sensor->readRawTemperature();
  auto temp = sensor->readTemperature();
  auto raw_pres = sensor->readRawPressure();
  auto pres = sensor->readPressure();
  auto alt = sensor->readAltitude();
  print(raw_temp, temp, raw_pres, pres, alt);
  
  std::cout << "Getting the information using the AnalogInputs:\n";
  raw_temp = sensor->rawTemperatureAnalogInput()->readValue();
  temp = sensor->temperatureAnalogInput()->readValue();
  raw_pres = sensor->rawPressureAnalogInput()->readValue();
  pres = sensor->pressureAnalogInput()->readValue();
  alt = sensor->altitudeAnalogInput()->readValue();
  print(raw_temp, temp, raw_pres, pres, alt);
  
  struct ScreenPrinter {
    ScreenPrinter() {
      print();
    }
    void print() {
      std::cout << raw_temp << ' ' << temp << ' ' << raw_pres << ' ' << pres << ' ' << alt << '\n';
    }
    void reprint() {
      print();
    }
    std::uint16_t raw_temp;
    float temp;
    std::uint32_t raw_pres;
    float pres;
    float alt;
  };
  ScreenPrinter printer {};
  auto screen_printer = std::make_shared<ScreenPrinter>();
  
  sensor->addRawTemperatureObserver(std::make_shared<PrinterUpdater<std::uint16_t, ScreenPrinter>>(printer.raw_temp, printer));
  sensor->addTemperatureObserver(std::make_shared<PrinterUpdater<float, ScreenPrinter>>(printer.temp, printer));
  sensor->addRawPressureObserver(std::make_shared<PrinterUpdater<std::uint32_t, ScreenPrinter>>(printer.raw_pres, printer));
  sensor->addPressureObserver(std::make_shared<PrinterUpdater<float, ScreenPrinter>>(printer.pres, printer));
  sensor->addAltitudeObserver(std::make_shared<PrinterUpdater<float, ScreenPrinter>>(printer.alt, printer));
  
  sensor->start();
  
  std::this_thread::sleep_for(10s);
  
}