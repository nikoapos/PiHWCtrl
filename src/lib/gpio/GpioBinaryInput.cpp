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
 * @file gpio/GpioBinaryInput.cpp
 * @author nikoapos
 */

#include <PiHWCtrl/utils/EventGenerator.h>
#include <PiHWCtrl/gpio/GpioBinaryInput.h>
#include <PiHWCtrl/gpio/exceptions.h>

namespace PiHWCtrl {

GpioBinaryInput::GpioBinaryInput(int gpio) : m_gpio(gpio, Gpio::Mode::INPUT) {
}

GpioBinaryInput::~GpioBinaryInput() {
  // Stop any threads generating events for this GPIO
  stop();
}

bool GpioBinaryInput::isOn() const {
  return m_gpio.getState();
}

void GpioBinaryInput::start(unsigned int sleep_ms) {
  if (m_observing) {
    throw GpioException() << "GPIO already started";
  }
  m_observing = true;
  auto event_func = [this]() {
    std::lock_guard<std::mutex> guard {m_gpio_mutex};
    return isOn();
  };
  auto notify_func = [this](bool value) {
    notifyObservers(value);
  };
  startEventGenerator<bool>(event_func, notify_func, m_observing, sleep_ms);
}

void GpioBinaryInput::stop() {
  if (m_observing) {
    // This will trigger the EventGenerator thread to stop
    m_observing = false;
    // We have to wait until the thread signals that it stopped
    while (!m_observing) {
    }
    // Now we can set again the flag to false
    m_observing = false;
  }
}

} // end of namespace PiHWCtrl