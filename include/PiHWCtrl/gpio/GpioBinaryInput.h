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
 * @file GpioBinaryInput.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_GPIO_GPIOBINARYINPUT_H
#define PIHWCTRL_GPIO_GPIOBINARYINPUT_H

#include <mutex>
#include <atomic>
#include <PiHWCtrl/HWInterfaces/BinaryInput.h>
#include <PiHWCtrl/HWInterfaces/Observable.h>
#include <PiHWCtrl/gpio/Gpio.h>

namespace PiHWCtrl {

/**
 * @class GpioBinaryInput
 * 
 * @brief
 * Implementation of the BinaryInput interface using the linux driver via sysfs
 * 
 * @details
 * This class can be used to access the GPIOs 2-28 of the 40 pin interface of
 * the Raspberry Pi. The input is interpreted as following:
 * 
 * - ON: 3.3 Volt connected to the pin
 * - OFF: GND connected to the pin or the pin is open circuited
 */
class GpioBinaryInput : public BinaryInput, public Observable<bool> {
  
public:
  
  /**
   * @brief Creates a GpioBinaryInput for the requested pin
   * 
   * @param gpio
   *    The number of the GPIO to use as the input
   * 
   * @throws GpioAlreadyResearved
   *    If the requested GPIO is already reserved by another PiHWCtrl object
   * @throws BadGpioNumber
   *    If the given number is out of the range 2-28
   * @throws GpioException
   *    If there was any problem with the communication with the driver
   */
  GpioBinaryInput(int gpio);

  /// Releases the physical GPIO
  virtual ~GpioBinaryInput();
  
  /// Returns true if the input is ON (as described at the class documentation)
  /// and false otherwise
  bool isOn() const override;
  
  void start(unsigned int sleep_ms=10);
  
  void stop();

private:
  
  Gpio m_gpio;
  mutable std::mutex m_gpio_mutex;
  std::atomic<bool> m_observing {false};
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_GPIO_GPIOBINARYINPUT_H */

