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
 * @file PigpioPWM.cpp
 * @author nikoapos
 */

#include <pigpio.h>
#include <PiHWCtrl/pigpio/SmartPigpio.h>
#include <PiHWCtrl/pigpio/exceptions.h>
#include <PiHWCtrl/pigpio/PigpioPWM.h>

namespace PiHWCtrl {

PigpioPWM::PigpioPWM(unsigned int gpio, unsigned int frequency) : m_gpio(gpio) {
  // Reserve the GPIO so no other class can use it
  SmartPigpio::getSingleton()->reserveGpio(m_gpio);
  
  // First we set the PWM frequency
  auto res = gpioSetPWMfrequency(m_gpio, frequency);
  if (res == PI_BAD_USER_GPIO) {
    throw BadGpioNumber(m_gpio);
  } else if (res <= 0) {
    throw UnknownPigpioException(res);
  }
  
  // After setting the frequency the real range is fixed. We set the range of the
  // PWM to the real one to get the maximum resolution we can.
  auto real_range = gpioGetPWMrealRange(m_gpio);
  if (real_range == PI_BAD_USER_GPIO) {
    throw BadGpioNumber(m_gpio);
  } else if (real_range < 0) {
    throw UnknownPigpioException(real_range);
  }
  res = gpioSetPWMrange(m_gpio, real_range);
  if (res == PI_BAD_USER_GPIO) {
    throw BadGpioNumber(m_gpio);
  } else if (res == PI_BAD_DUTYRANGE) {
    throw BadPWMDutyRange(m_gpio, real_range);
  } else if (res != real_range) {
    throw UnknownPigpioException(res);
  }
  
  // Now that everything is set, we start the PWM on the GPIO with 0 duty cycle (off)
  res = gpioPWM(m_gpio, 0);
  if (res == PI_BAD_USER_GPIO) {
    throw BadGpioNumber(m_gpio);
  } else if (res == PI_BAD_DUTYCYCLE) {
    throw BadPWMDutyCycle(m_gpio, 0);
  } else if (res != 0) {
    throw UnknownPigpioException(res);
  }
  
}

PigpioPWM::~PigpioPWM() {
  // We turn off the PWM
  gpioPWM(m_gpio, 0);
}

void PigpioPWM::setDutyCycle(float duty_cycle) {
  float range = gpioGetPWMrange(m_gpio);
  if (range == PI_BAD_USER_GPIO) {
    throw BadGpioNumber(m_gpio);
  }
  
  unsigned int int_duty_cycle = range * duty_cycle;
  auto res = gpioPWM(m_gpio, int_duty_cycle);
  if (res == PI_BAD_USER_GPIO) {
    throw BadGpioNumber(m_gpio);
  } else if (res == PI_BAD_DUTYCYCLE) {
    throw BadPWMDutyCycle(m_gpio, duty_cycle);
  } else if (res != 0) {
    throw UnknownPigpioException(res);
  }
  
}

float PigpioPWM::getDutyCycle() {
  float range = gpioGetPWMrange(m_gpio);
  if (range == PI_BAD_USER_GPIO) {
    throw BadGpioNumber(m_gpio);
  }
  
  float duty_cycle = gpioGetPWMdutycycle(m_gpio);
  if (duty_cycle == PI_BAD_USER_GPIO) {
    throw BadGpioNumber(m_gpio);
  } else if (duty_cycle == PI_NOT_PWM_GPIO) {
    throw NotPWMGpio(m_gpio);
  }
  
  return duty_cycle / range;
}

} // end of namespace PiHWCtrl