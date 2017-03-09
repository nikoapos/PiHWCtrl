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
 * @file Gpio.cpp
 * @author nikoapos
 */

#include <string>
#include <fstream>
#include <map>
#include <chrono> // for std::chrono_literals
#include <thread> // for std::this_thread
#include <boost/filesystem.hpp>
#include <PiHWCtrl/utils/GpioManager.h>
#include <PiHWCtrl/gpio/Gpio.h>
#include "PiHWCtrl/gpio/exceptions.h"

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

namespace PiHWCtrl {

namespace {

const std::string gpio_path {"/sys/class/gpio"};
const std::string gpio_export {gpio_path + "/export"};
const std::string gpio_unexport {gpio_path + "/unexport"};

std::map<Gpio::Mode, std::string> mode_map {
  {Gpio::Mode::INPUT, "in"},
  {Gpio::Mode::OUTPUT, "out"}
};

} // end of anonymous namespace

Gpio::GpioExporter::GpioExporter(int gpio_no) : m_gpio_no(gpio_no) {
  // Check that the GPIO is not already exported
  std::string gpio_dir {gpio_path + "/gpio" + std::to_string(m_gpio_no)};
  if (boost::filesystem::exists(gpio_dir)) {
    throw GpioException() << "GPIO " << m_gpio_no << " is already exported";
  }
  
  // Export the GPIO by writing its number to the export file
  {
    std::ofstream out {gpio_export};
    out << m_gpio_no;
  }
  
  // Give some time to the driver to initialize everything
  std::this_thread::sleep_for(50ms);
  
  // Check that the GPIO is exported correctly
  if (!boost::filesystem::exists(gpio_dir)) {
    throw GpioException() << "Failed to export GPIO " << m_gpio_no;
  }
}

Gpio::GpioExporter::~GpioExporter() {
  // Unexport the GPIO by writing its number to the unexport file
  {
    std::ofstream out {gpio_unexport};
    out << m_gpio_no;
  }
}

Gpio::Gpio(int gpio_no, Mode mode) {
  m_gpio_reservation = GpioManager::getSingleton()->reserveGpio(gpio_no);
  m_gpio_exporter = std::make_unique<GpioExporter>(gpio_no);
  
  // Set the mode
  std::string gpio_dir {gpio_path + "/gpio" + std::to_string(gpio_no)};
  std::string direction_file {gpio_dir + "/direction"};
  {
    std::ofstream out {direction_file};
    out << mode_map[mode];
  }
  
  // Store the name of the value file that will be used
  m_value_file = gpio_dir + "/value";
}

bool Gpio::getState() const {
  std::ifstream in {m_value_file};
  auto value = in.get();
  return (value == '0') ? false : true;
}

void Gpio::setState(bool state) {
  std::ofstream out {m_value_file};
  if (state) {
    out << "1";
  } else {
    out << "0";
  }
}

} // end of namespace PiHWCtrl