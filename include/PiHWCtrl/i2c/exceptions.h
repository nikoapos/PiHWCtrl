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
 * @file i2c/exceptions.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_I2C_EXCEPTIONS_H
#define PIHWCTRL_I2C_EXCEPTIONS_H

#include <cerrno>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <PiHWCtrl/HWInterfaces/exceptions.h>

namespace PiHWCtrl {

class I2CBusOpenFailure : public Exception {
public:
  I2CBusOpenFailure(std::string bus_name) : bus_name(bus_name), err_code(errno) {
    appendMessage("Failed to open I2C bus " + bus_name + ": ");
    appendMessage(std::strerror(err_code));
  }
  std::string bus_name;
  int err_code;
};

class I2CDeviceConnectionFailure : public Exception {
public:
  I2CDeviceConnectionFailure(int address) : address(address), err_code(errno) {
    appendMessage("Failed to connect to I2C address ");
    std::stringstream address_str;
    address_str << std::hex << address;
    appendMessage(address_str.str() + ": ");
    appendMessage(std::strerror(err_code));
  }
  int address;
  int err_code;
};

class I2CActionOutOfTransaction : public Exception {
};

class I2CReadRegisterException : public Exception {
public:
  I2CReadRegisterException(std::int8_t register_address) 
          : register_address(register_address), err_code(errno) {
    appendMessage("Failed to read register ");
    std::stringstream address_str;
    address_str << std::hex << (int)register_address;
    appendMessage(address_str.str() + ": ");
    appendMessage(std::strerror(err_code));
  }
  std::int8_t register_address;
  int err_code;
};

template <typename T>
class I2CWriteRegisterException : public Exception {
public:
  I2CWriteRegisterException(std::int8_t register_address, T value)
          : register_address(register_address), err_code(errno), value(value) {
    std::stringstream message;
    message << "Failed to write " << value << " in register "
            << (int)register_address << ": ";
    appendMessage(message.str());
    appendMessage(std::strerror(err_code));
  }
  std::int8_t register_address;
  int err_code;
  T value;
};

class I2CWrongModule : public Exception {
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_I2C_EXCEPTIONS_H */

