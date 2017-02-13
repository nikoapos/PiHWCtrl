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

#ifndef PIHECTRL_EXCEPTIONS_H
#define PIHECTRL_EXCEPTIONS_H

#include <cerrno>
#include <cstring>
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

} // end of namespace PiHWCtrl

#endif /* PIHECTRL_EXCEPTIONS_H */

