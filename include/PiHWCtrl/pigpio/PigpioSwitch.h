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
 * @file PiHWCtrl/pigpio/PigpioSwitch.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_PIGPIOSWITCH_H
#define PIHWCTRL_PIGPIOSWITCH_H

#include <PiHWCtrl/HWInterfaces/Switch.h>
#include <PiHWCtrl/HWInterfaces/BinaryInput.h>

namespace PiHWCtrl {

/**
 * @class PigpioSwitch
 * 
 * @brief
 * Implemenation of the Switch interface using the pigpio library
 * 
 * @details
 * This class can be used to control the GPIOs 2-28 of the 40 pin interface of
 * the Raspberry Pi as ON (3.3 Volt) / OFF (GND) switches. The class also
 * implements the BinaryInput interface so its current state can be retrieved
 * from the code side.
 * 
 * Any program using this class must be executed with root privileges (sudo).
 */
class PigpioSwitch : public Switch, public BinaryInput {
  
public:

  /**
   * @brief Creates a PigipioSwitch for a specific GPIO pin
   *
   * @details
   * After the constructor has finished the related pin mode is set to output.
   * Note that this class can only be used for the GPIOs accessible via the 40
   * pin interface (GPIOs 2-28).
   *  
   * @param gpio
   *    The number of the GPIO to control with the switch
   * 
   * @throws GpioAlreadyResearved
   *    If the requested GPIO is already reserved by another PiHWCtrl object
   * @throws BadGpioNumber
   *    If the given number is out of the range 2-28 or if the pigpio call
   *    returns PI_BAD_GPIO
   * @throws BadGpioMode
   *    If the pigpio call returns PI_BAD_MODE
   * @throws UnknownPigpioException
   *    If the pigpio call returns any other error
   */
  PigpioSwitch(unsigned int gpio);
  
  /**
   * @brief Destructor of the PigpioSwitch
   * 
   * @details
   * After the destructor is called the GPIO reserved by it is released and it
   * can be used by other PiHWCtrl objects.
   */
  virtual ~PigpioSwitch();

  /// Sets the GPIO to ON (3.3 Volt) if the parameter is true or OFF (GRD) if
  /// it is false.
  void set(bool value) override;
  
  /// Returns true if the switch is set to ON and false if it is set to OFF.
  bool isOn() const override;
  
private:
  
  unsigned int m_gpio;

};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_PIGPIOSWITCH_H */

