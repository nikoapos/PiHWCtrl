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
 * @file PiHWCtrl/modules/HCSR04.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_HCSR04_H
#define PIHWCTRL_HCSR04_H

#include <memory>
#include <mutex>
#include <atomic>
#include <PiHWCtrl/HWInterfaces/BinaryInput.h>
#include <PiHWCtrl/HWInterfaces/Switch.h>
#include <PiHWCtrl/HWInterfaces/AnalogInput.h>
#include <PiHWCtrl/HWInterfaces/Observable.h>

namespace PiHWCtrl {

/**
 * @class HCSR04
 * 
 * @brief
 * Class for controlling the HC-SR04 ultrasonic distance measurement sensor
 * 
 * @details
 * The HC-SR04 measures distances by producing 8 ultrasound pulses at 40 kHz and
 * measuring the time required for the pulses to be reflected by an object and
 * come back. The way the HC-SR04 works is by using the two pins named Trig and
 * Echo. A short pulse of 10us at the Trig pin starts a measurement. The HC-SR04
 * sends the ultrasound pulses and waits for them to return. After the pulses
 * return, it sets the Echo pin up for as much time as it took to the pulses to
 * return. Note that if the pulses are not returned, the Echo pin will not be
 * raised at all.
 * 
 * The HCSR04 class communicates with the device via a Switch for triggering the
 * measurements and a BinaryInput for measuring the echo. Note that the class
 * handles the case of no echo raised (no pulses returned), by returning the
 * maximum distance. 
 */
class HCSR04 : public AnalogInput<float>, public Observable<float> {
  
public:
  
  /**
   * @brief Creates an HCSR04 instance
   * 
   * @details
   * In most of the cases you most probably want to use the GPIO pins for the
   * trigger and echo parameters. If this is the case, you can use the classes
   * in the PiHWCtrl/gpio package.
   * 
   * Any distance measured bigger than the parameter max_distance is going to
   * be trimmed to this value. The default is set to 4 meters, which is the
   * maximum range of the device. Note that this value will also be returned in
   * the case the device does not detect any pulses coming back.
   * 
   * @param trigger
   *    The object to receive the pulses to start the measurement
   * @param echo
   *    The object to read the echo delay from
   * @param max_distance
   *    The maximum distance to measure (in meters)
   * @param sound_speed
   *    The speed of sound to use (in meters/seconds)
   */
  HCSR04(std::unique_ptr<Switch> trigger, std::unique_ptr<BinaryInput> echo,
         float max_distance=4., float sound_speed=343);

  /// Destructor
  virtual ~HCSR04();
  
  /**
   * @brief Measures the distance using the sensor
   * 
   * @details
   * The time this method will block depends on the distance measured, as it
   * depends on the time the ultrasound pulses need to travel back and forth.
   * 
   * @return The measured distance
   */
  float readDistance();
  
  /// Redirects the call to the readDistance()
  float readValue() override;
  
  /**
   * @brief Enter continuous measurement mode
   * 
   * @details
   * In this mode, the class will repeatedly use the sensor for measuring the
   * distance and it will notify all the registered observers. The parameter
   * is the idle time between performing two measurements and it can be used to
   * guarantee that a consequent measurement is not affected by reflections of
   * a previous one.
   * 
   * @param sleep_ms
   *    The idle time between measurements (in milliseconds)
   */
  void start(unsigned int sleep_ms=60);
  
  /// Stop the continuous measurement mode
  void stop();
  
private:
  
  std::unique_ptr<Switch> m_trigger;
  std::unique_ptr<BinaryInput> m_echo;
  float m_max_dist;
  float m_sound_speed;
  mutable std::mutex m_mutex;
  std::atomic<bool> m_observing {false};
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_HCSR04_H */

