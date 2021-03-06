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

/**
 * @file PiHWCtrl/pigpio/PigpioBinaryInput.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_PIGPIOBINARYINPUT_H
#define PIHWCTRL_PIGPIOBINARYINPUT_H

#include <PiHWCtrl/HWInterfaces/BinaryInput.h>
#include <PiHWCtrl/utils/GpioManager.h>
#include <PiHWCtrl/pigpio/SmartPigpio.h>

namespace PiHWCtrl {

/**
 * @class PigpioBinaryInput
 * 
 * @brief
 * Implementation of the BinaryInput interface using the pigpio library
 * 
 * @details
 * This class can be used to access the GPIOs 2-28 of the 40 pin interface of
 * the Raspberry Pi. The input is interpreted as following:
 * 
 * - ON: 3.3 Volt connected to the pin
 * - OFF: GND connected to the pin or the pin is open circuited
 * 
 * Any program using this class must be executed with root privileges (sudo).
 */
class PigpioBinaryInput : public BinaryInput {
  
public:
  
  /**
   * @brief Creates a PigpioBinaryInput for a specific GPIO pin
   * 
   * @details
   * After the constructor has finished the related pin mode is set to input.
   * Note that this class can only be used for the GPIOs accessible via the 40
   * pin interface (GPIOs 2-28).
   * 
   * @param gpio
   *    The number of the GPIO to use as the input
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
  PigpioBinaryInput(int gpio);
  
  // A PigpioBinaryInput represents a physical GPIO, so it cannot be copied
  PigpioBinaryInput(const PigpioBinaryInput& other) = delete;
  PigpioBinaryInput& operator=(const PigpioBinaryInput& right) = delete;
  
  // Moving is OK. The old object will not manage the GPIO any more.
  PigpioBinaryInput(PigpioBinaryInput&& other) = default;
  PigpioBinaryInput& operator=(PigpioBinaryInput&& other) = default;
  
  /**
   * @brief Destructor of the PigpioBinaryInput
   * 
   * @details
   * After the destructor is called the GPIO reserved by it is released and it
   * can be used by other PiHWCtrl objects.
   */
  virtual ~PigpioBinaryInput() = default;
  
  /// Returns true if the input is ON (as described at the class documentation)
  bool isOn() const override;
  
private:
  
  int m_gpio = -1;
  // We keep a pointer to the SmartPigpio singleton to guarantee that it is
  // initialized and not deleted for the lifetime of the object
  std::shared_ptr<SmartPigpio> m_smart_pigpio = SmartPigpio::getSingleton();
  std::unique_ptr<GpioManager::GpioReservation> m_gpio_reservation;

};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_PIGPIOBINARYINPUT_H */

