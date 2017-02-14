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
#include <PiHWCtrl/modules/BMP180.h>

int main() {
  PiHWCtrl::BMP180 sensor {PiHWCtrl::BMP180::PressureMode::ULTRA_LOW_POWER};
  
  sensor.calibrateSeaLevelPressure(480);
  
  std::cout << "The temperature is " << sensor.readTemperature() << " C\n";
  std::cout << "The pressure is " << sensor.readPressure() << " hPa\n";
  std::cout << "The altitude is " << sensor.readAltitude() << " m\n";
  
  auto analog_input = sensor.temperatureAnalogInput();
  std::cout << "Temperature from analog input is " << analog_input->readValue() << " C\n";
  analog_input = sensor.pressureAnalogInput();
  std::cout << "Pressure from analog input is " << analog_input->readValue() << " C\n";
  analog_input = sensor.altitudeAnalogInput();
  std::cout << "Altitude from analog input is " << analog_input->readValue() << " C\n";
}