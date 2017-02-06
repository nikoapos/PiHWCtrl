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
 * @file exceptions.h
 * @author nikoapos
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

namespace PiHWCtrl {

class UnknownPigpioException : public std::exception {
public:
  UnknownPigpioException(int err_code) : exception(), err_code(err_code) {}
  int err_code;
};

class PigpioInitFailed : public std::exception {
public:
  PigpioInitFailed(int err_code) : exception(), err_code(err_code) {}
  int err_code;
};

class BadGpioNumber : public std::exception {
public:
  BadGpioNumber(unsigned int gpio) : exception(), gpio(gpio) {}
  unsigned int gpio;
};

class BadGpioMode : public std::exception {
public:
  BadGpioMode(unsigned int gpio, unsigned int mode) : exception(), gpio(gpio), mode(mode) {}
  unsigned int gpio;
  unsigned int mode;
};

class GpioAlreadyReserved : public std::exception {
public:
  GpioAlreadyReserved(unsigned int gpio) : exception(), gpio(gpio) {}
  unsigned int gpio;
};

} // end of namespace PiHWCtrl

#endif /* EXCEPTIONS_H */

