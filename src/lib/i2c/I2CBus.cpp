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
 * @file i2c/I2CBus.cpp
 * @author nikoapos
 */

#include <string>
#include <fcntl.h> // For open()
#include <unistd.h> // For close()
#include <sys/ioctl.h> // For ioctl()
#include <linux/i2c-dev.h>
#include <PiHWCtrl/utils/GpioManager.h>
#include <PiHWCtrl/i2c/I2CBus.h>
#include <PiHWCtrl/i2c/exceptions.h>

namespace PiHWCtrl {

namespace {

constexpr int I2C_ADAPTER = 1;
constexpr int SDA_GPIO = 2;
constexpr int SCL_GPIO = 3;

void connectToDevice(int bus_file, int address) {
  if (ioctl(bus_file, I2C_SLAVE, address) < 0) {
    throw I2CDeviceConnectionFailure(address);
  }
}

} // end of anonymous namespace

std::shared_ptr<I2CBus> I2CBus::getSingleton() {
  static std::shared_ptr<I2CBus> singleton = std::shared_ptr<I2CBus>(new I2CBus{});
  return singleton;
}

I2CBus::I2CBus() {
  // Reserve the GPIOs used for the SDA and SCL so no other object can use them
  GpioManager::getSingleton()->reserveGpio(SDA_GPIO);
  GpioManager::getSingleton()->reserveGpio(SCL_GPIO);
  
  // Open the file for using the bus
  std::string filename = "/dev/i2c-" + std::to_string(I2C_ADAPTER);
  m_bus_file = open(filename.c_str(), O_RDWR);
  if (m_bus_file < 0) {
    throw I2CBusOpenFailure(filename);
  }
}

I2CBus::~I2CBus() {
  // Release the GPIOs so other objects can use them
  GpioManager::getSingleton()->releaseGpio(SDA_GPIO);
  GpioManager::getSingleton()->releaseGpio(SCL_GPIO);
  
  // Close the bus file
  close(m_bus_file);
}

I2CTransaction I2CBus::startTransaction(std::uint8_t address) {
  I2CTransaction transaction {m_bus_mutex};
  connectToDevice(m_bus_file, address);
  return transaction;
}

} // end of namespace PiHWCtrl