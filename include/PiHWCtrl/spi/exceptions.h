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
 * @file PiHWCtrl/spi/exceptions.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_SPI_EXCEPTIONS_H
#define PIHWCTRL_SPI_EXCEPTIONS_H

#include <cerrno>
#include <cstring>
#include <cstdint>
#include <PiHWCtrl/HWInterfaces/exceptions.h>

namespace PiHWCtrl {

class SPIBusOpenFailure : public Exception {
public:
  SPIBusOpenFailure(std::string bus_name) : bus_name(bus_name), err_code(errno) {
    appendMessage("Failed to open SPI bus " + bus_name + ": ");
    appendMessage(std::strerror(err_code));
  }
  std::string bus_name;
  int err_code;
};

class SPISpeedException : public Exception {
public:
  SPISpeedException(std::uint32_t speed) : speed(speed), err_code(errno) {
    appendMessage("Failed to set the SPI bus speed to ");
    appendMessage(speed);
    appendMessage(": ");
    appendMessage(std::strerror(err_code));
  }
  std::uint32_t speed;
  int err_code;
};

class SPIModeException : public Exception {
public:
  SPIModeException(std::uint8_t mode) : mode(mode), err_code(errno) {
    appendMessage("Failed to set the SPI bus mode to ");
    appendMessage((int)mode);
    appendMessage(": ");
    appendMessage(std::strerror(err_code));
  }
  std::uint8_t mode;
  int err_code;
};

class SPIBitsPerWordException : public Exception {
public:
  SPIBitsPerWordException(std::uint8_t bits) : bits(bits), err_code(errno) {
    appendMessage("Failed to set the SPI bus bits per word to ");
    appendMessage((int)bits);
    appendMessage(": ");
    appendMessage(std::strerror(err_code));
  }
  std::uint8_t bits;
  int err_code;
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_SPI_EXCEPTIONS_H */

