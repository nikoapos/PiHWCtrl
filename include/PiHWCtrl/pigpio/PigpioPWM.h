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
 * @file PiHWCtrl/pigpio/PigpioPWM.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_PIGPIOPWM_H
#define PIHWCTRL_PIGPIOPWM_H

#include <PiHWCtrl/HWInterfaces/PWM.h>
#include <PiHWCtrl/pigpio/SmartPigpio.h>

namespace PiHWCtrl {

/**
 * @class PigpioPWM
 * 
 * @brief
 * Implementation of the PWM interface using the pigpio library for semi-hardware
 * PWM (using the DMA)
 * 
 * 
 * @details
 * This class can be used to send PWM signal to any of the GPIOs 2.28 of the
 * 40 pin interface of the Raspberry Pi. The signal is NOT a hardware PWM, but
 * a semi-hardware PWM using DMA, as provided by the pigpio library.
 * 
 * Warning: This class will use the DMA driven PWM even for the GPIOs which
 * support hardware PWM. If the DMA PWM is not sufficient for your application
 * you should use the PigpioHardwarePWM class instead.
 * 
 * Any program using this class must be executed with root privileges (sudo).
 */
class PigpioPWM : public PWM {
  
public:
  
  /**
   * @brief Creates a PigpioPWM for a specific GPIO pin
   * 
   * @details
   * The constructor gets as parameter the frequency of the PWM. The rules of the
   * accepted values for the frequency are the same with the pigpio library and
   * can be summarized as following:
   * 
   * - There are 18 different PWM frequencies
   * - The value given by the user is rounded to the closest of the 18 available ones
   * - The available frequencies depend upon the sample rate
   * 
   * For more information you should refer to the pigpio documentation here:
   * http://abyz.co.uk/rpi/pigpio/cif.html#gpioSetPWMfrequency .
   * 
   * When setting the frequency keep in mind that higher frequencies will reduce
   * the range of different values for the duty cycle, resulting to less accurate
   * control. If the combination of high frequency and accurate control is
   * necessary for your application you should consider to use a hardware PWM
   * instead.
   * 
   * @param gpio
   *    The number of the GPIO to send the PWM to
   * @param frequency
   *    The frequency of the PWM
   * 
   * @throws GpioAlreadyResearved
   *    If the requested GPIO is already reserved by another PiHWCtrl object
   * @throws BadGpioNumber
   *    If the given number is out of the range 2-28 or if the pigpio call
   *    returns PI_BAD_USER_GPIO
   * @throws BadPWMDutyRange
   *    If the pigpio call returns PI_BAD_DUTYRANGE
   * @throws BadPWMDutyCycle
   *    If the pigpio call returns PI_BAD_DUTYCYCLE
   * @throws UnknownPigpioException
   *    If the pigpio call returns any other error
   */
  PigpioPWM(int gpio, unsigned int frequency=800);
  
  // A PigpioPWM represents a physical GPIO, so it cannot be copied
  PigpioPWM(const PigpioPWM& other) = delete;
  PigpioPWM& operator=(const PigpioPWM& right) = delete;
  
  // Moving is OK. The old object will not manage the GPIO any more.
  PigpioPWM(PigpioPWM&& other);
  PigpioPWM& operator=(PigpioPWM&& other);

  /// The destructor will turn off the PWM
  virtual ~PigpioPWM();
  
  /**
   * @brief Sets the duty cycle
   * 
   * @details
   * The given parameter must be a value in the range [0, 1] where 0 means
   * fully off and 1 means fully on.
   * 
   * @param duty_cycle
   *    The duty cycle to apply
   * 
   * @throws BadGpioNumber
   *    If the pigpio call returns PI_BAD_USER_GPIO
   * @throws BadPWMDutyCycle
   *    If the pigpio call returns PI_BAD_DUTYCYCLE
   * @throws UnknownPigpioException
   *    If the pigpio call returns any other error
   */
  void setDutyCycle(float duty_cycle) override;
  
  /**
   * @brief Returns the current duty cycle
   * 
   * @return The current duty cycle
   * 
   * @throws BadGpioNumber
   *    If the pigpio call returns PI_BAD_USER_GPIO
   * @throws NotPWMGpio
   *    If the pigpio call returns PI_NOT_PWM_GPIO
   */
  float getDutyCycle() override;
  
private:
  
  int m_gpio = -1;
  // We keep a pointer to the SmartPigpio singleton to guarantee that it is
  // initialized and not deleted for the lifetime of the object
  std::shared_ptr<SmartPigpio> m_smart_pigpio = SmartPigpio::getSingleton();
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_PIGPIOPWM_H */

