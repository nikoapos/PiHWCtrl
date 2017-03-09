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
 * @file PiHWCtrl/gpio/Gpio.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_GPIO_GPIO_H
#define PIHWCTRL_GPIO_GPIO_H

#include <string>
#include <PiHWCtrl/utils/GpioManager.h>

namespace PiHWCtrl {

/**
 * @class Gpio
 * 
 * @brief Controller class for a hardware GPIO pin
 * 
 * @details
 * This class can be used to control the GPIOs 2-28 of the 40 pin interface of
 * the Raspberry Pi. This is achieved by using the linux driver via sysfs.
 * Each GPIO is considered to have a boolean state which has the following
 * meaning:
 * 
 * - true : ON - 3V3 connected to the pin
 * - false : OFF - GND connected to the pin
 * 
 * This class can be used to control a GPIO both as input and as output.
 */
class Gpio {
  
public:
  
  enum class Mode {
    INPUT, OUTPUT
  };

  /**
   * @brief Constructs a new Gpio object
   * @details
   * The newly created object manages the GPIO with the given number. Note that
   * there can be a single instance of this class for each GPIO. The mode
   * parameter controls if a GPIO is set as input or output. GPIOs which are
   * set as inputs cannot use the method setState().
   * @param gpio_no The GPIO to control
   * @param mode The mode of the GPIO
   */
  Gpio(int gpio_no, Mode mode=Mode::INPUT);
  
  // A Gpio represents a physical GPIO, so it cannot be copied
  Gpio(const Gpio&) = delete;
  Gpio& operator=(const Gpio&) = delete;
  
  // Moving is OK. The old object will not manage the GPIO any more.
  Gpio(Gpio&& other) = default;
  Gpio& operator=(Gpio&& other) = default;

  virtual ~Gpio() = default;
  
  /// Returns the state of the GPIO
  bool getState() const;
  
  /// Sets the state of the GPIO (only for output GPIOs)
  void setState(bool state);
  
private:
  
  struct GpioExporter {
    GpioExporter(int gpio_no);
    virtual ~GpioExporter();
    int m_gpio_no;
  };
  
  std::unique_ptr<GpioManager::GpioReservation> m_gpio_reservation;
  std::unique_ptr<GpioExporter> m_gpio_exporter;
  std::string m_value_file;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_GPIO_GPIO_H */

