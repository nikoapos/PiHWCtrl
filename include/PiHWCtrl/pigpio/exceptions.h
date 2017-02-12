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

#ifndef PIHWCTRL_PIGPIO_EXCEPTIONS_H
#define PIHWCTRL_PIGPIO_EXCEPTIONS_H

#include <PiHWCtrl/HWInterfaces/exceptions.h>

namespace PiHWCtrl {

class UnknownPigpioException : public Exception {
public:
  UnknownPigpioException(int err_code) : err_code(err_code) {
    appendMessage("Unknown PIGPIO exception with code ");
    appendMessage(err_code);
  }
  int err_code;
};

class PigpioInitFailed : public Exception {
public:
  PigpioInitFailed(int err_code) : err_code(err_code) {
    appendMessage("PIGPIO initialization failed with code ");
    appendMessage(err_code);
  }
  int err_code;
};

class BadGpioMode : public Exception {
public:
  BadGpioMode(int gpio, unsigned int mode) : gpio(gpio), mode(mode) {
    appendMessage("Bad GPIO mode: GPIO = ");
    appendMessage(gpio);
    appendMessage(", mode = ");
    appendMessage(mode);
  }
  int gpio;
  unsigned int mode;
};

class BadPWMDutyRange : public Exception {
public:
  BadPWMDutyRange(int gpio, unsigned int range) : gpio(gpio), range(range) {
    appendMessage("Bad PWM duty range: GPIO = ");
    appendMessage(gpio);
    appendMessage(", range = ");
    appendMessage(range);
  }
  int gpio;
  unsigned int range;
};

class BadPWMDutyCycle : public Exception {
public:
  BadPWMDutyCycle(int gpio, float duty_cycle) : gpio(gpio), duty_cycle(duty_cycle) {
    appendMessage("Bad PWM duty cycle: GPIO = ");
    appendMessage(gpio);
    appendMessage(", duty cycle = ");
    appendMessage(duty_cycle);
  }
  int gpio;
  float duty_cycle;
};

class NotPWMGpio : public Exception {
public:
  NotPWMGpio(int gpio) : gpio(gpio) {
    appendMessage("GPIO ");
    appendMessage(gpio);
    appendMessage(" is not a setup for PWM");
  }
  int gpio;
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_PIGPIO_EXCEPTIONS_H */

