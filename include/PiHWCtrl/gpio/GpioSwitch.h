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
 * @file GpioSwitch.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_GPIO_GPIOSWITCH_H
#define PIHWCTRL_GPIO_GPIOSWITCH_H

#include <mutex>
#include <atomic>
#include <PiHWCtrl/HWInterfaces/Switch.h>
#include <PiHWCtrl/HWInterfaces/BinaryInput.h>
#include <PiHWCtrl/HWInterfaces/Observable.h>
#include <PiHWCtrl/gpio/Gpio.h>

namespace PiHWCtrl {

/**
 * @class GpioSwitch
 * 
 * @brief
 * Implemenation of the Switch interface using the linux driver via sysfs
 * 
 * @details
 * This class can be used to control the GPIOs 2-28 of the 40 pin interface of
 * the Raspberry Pi as ON (3.3 Volt) / OFF (GND) switches. The class also
 * implements the BinaryInput and Observable interfaces so its current state can
 * be retrieved from the code side. Note that the class generates events only
 * when the set() method is called and it is not continuously monitored.
 */
class GpioSwitch : public Switch, public BinaryInput, public Observable<bool> {
  
public:
  
  /**
   * @brief Creates a GpioSwitch for the requested pin
   * 
   * @param gpio
   *    The number of the GPIO to use as the switch
   * 
   * @throws GpioAlreadyResearved
   *    If the requested GPIO is already reserved by another PiHWCtrl object
   * @throws BadGpioNumber
   *    If the given number is out of the range 2-28
   * @throws GpioException
   *    If there was any problem with the communication with the driver
   */
  GpioSwitch(int gpio);
  
  /// Releases the physical GPIO
  virtual ~GpioSwitch() = default;
  
  /// Sets the state of the pin (as described at the class documentation)
  void set(bool value) override;
  
  /// Returns true if the input is ON (as described at the class documentation)
  /// and false otherwise
  bool isOn() const override;
  
private:
  
  Gpio m_gpio;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_GPIO_GPIOSWITCH_H */

