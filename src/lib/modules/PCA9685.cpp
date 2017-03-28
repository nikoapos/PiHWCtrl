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
 * @file PCA9685.cpp
 * @author nikoapos
 */

#include <chrono> // for std::chrono_literals
#include <thread>
#include <map>
#include <cmath>
#include <functional>

#include <PiHWCtrl/i2c/I2CBus.h>
#include <PiHWCtrl/i2c/exceptions.h>
#include <PiHWCtrl/modules/PCA9685.h>
#include <PiHWCtrl/modules/exceptions.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

namespace PiHWCtrl {

namespace {

// Registers
constexpr std::uint8_t REG_MODE1 = 0x00;
constexpr std::uint8_t REG_MODE2 = 0x01;
constexpr std::uint8_t REG_LED_ON = 0x06;
constexpr std::uint8_t REG_LED_OFF = 0x08;
constexpr std::uint8_t REG_ALL_LED_ON = 0xFA;
constexpr std::uint8_t REG_ALL_LED_OFF = 0xFC;
constexpr std::uint8_t REG_PRE_SCALE = 0xFE;

constexpr std::uint8_t LED_SHIFT = 0x04;

// Commands
constexpr std::uint8_t CMD_AUTO_INCR = 0x20;
constexpr std::uint16_t CMD_LED_FULL_ON = 0x1000;
constexpr std::uint16_t CMD_LED_FULL_OFF = 0x1000;

std::mutex instance_exists_mutex;

std::map<std::uint8_t, bool> instance_exist_map {
};

class LedPwm : public PWM {
  
public:
  
  LedPwm(PCA9685& device, int led) : m_device(device), m_led(led) {
  }

  virtual ~LedPwm() = default;
  
  void setDutyCycle(float duty_cycle) override {
    m_device.get().setDutyCycle(m_led, duty_cycle);
  }
  
  float getDutyCycle() override {
    m_device.get().getDutyCycle(m_led);
  }

private:
  
  std::reference_wrapper<PCA9685> m_device;
  int m_led;
  
};

} // end of anonymous namespace

std::unique_ptr<PCA9685> PCA9685::factory(std::uint8_t address, int pwm_frequency) {
  return std::unique_ptr<PCA9685>{new PCA9685{address, pwm_frequency}};
}

PCA9685::PCA9685(std::uint8_t address, int pwm_frequency) : m_address(address) {
  
  // First check that the pwm_frequency is in the valid range
  if (pwm_frequency < 24 || pwm_frequency > 1526) {
    throw Exception() << "Invalid PWM frequency " << pwm_frequency << " (msut be"
            << " in range [24, 1526])";
  }

  // Check that there is no other instance controlling this address
  std::unique_lock<std::mutex> lock {instance_exists_mutex};
  auto instance_iter = instance_exist_map.find(m_address);
  if (instance_iter != instance_exist_map.end() && instance_iter->second) {
    throw ModuleAlreadyInUse("PCA9685-"+m_address);
  } else {
    // This will either change the value of an existing key or create a new entry
    instance_exist_map[m_address] = true;
  }
  lock.unlock();
  
  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // First initialize and wake up the device. We set the auto-increment on so
  // we can read the registers as 16 bit integers, we set the sleep to off to
  // wake up the device and we turn off the ALLCALL.
  {
    auto transaction = bus->startTransaction(m_address);
    std::uint8_t cmd = 0x00;
    cmd |= CMD_AUTO_INCR;
    bus->writeRegister(REG_MODE1, cmd);
  }
  
  // Sleep for 500us for the oscillator to stabilize
  std::this_thread::sleep_for(500us);
  
  // Set all the LEDs to full OFF and all their registers to zero values
  auto transaction = bus->startTransaction(m_address);
  bus->writeRegister(REG_ALL_LED_ON, 0x0000);
  bus->writeRegister(REG_ALL_LED_OFF, CMD_LED_FULL_OFF);
  
  // Set the PRE_SCALE for the requested frequency
  std::uint8_t prescale = std::round(25e6 / (4096. * pwm_frequency)) -1;
  bus->writeRegister(REG_PRE_SCALE, prescale);
  
} // end of PCA9685 constructor

PCA9685::~PCA9685() {
  // Release the instance_exists flag so new classes can be created
  std::lock_guard<std::mutex> lock {instance_exists_mutex};
  instance_exist_map.at(m_address) = false;
}

void PCA9685::setDutyCycle(int channel, float duty_cycle) {
  
  if (channel < 0 || channel > 15) {
    throw Exception() << "Invalid channel number " << channel;
  }
  if (duty_cycle < 0 || duty_cycle > 1) {
    throw Exception() << "Invalid duty cycle " << duty_cycle;
  }
  
  std::uint8_t led_on_reg = REG_LED_ON + channel * LED_SHIFT;
  std::uint8_t led_off_reg = REG_LED_OFF + channel * LED_SHIFT;
  
  std::uint16_t on = 0x0000;
  std::uint16_t off = 0x0000;
  
  // If the duty cycle is 0 or 1 we set the full ON or full OFF
  if (duty_cycle == 0) {
    off = CMD_LED_FULL_OFF;
  } else if (duty_cycle == 1) {
    on = CMD_LED_FULL_ON;
  } else {
    off = std::int16_t(duty_cycle * 4096) & 0x0FFF;
  }
  
  std::lock_guard<std::mutex> lock {m_mutex};

  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  auto transaction = bus->startTransaction(m_address);
  
  // Write the registers
  bus->writeRegister(led_on_reg, on);
  bus->writeRegister(led_off_reg, off);
  
} // end of setDutyCycle()

float PCA9685::getDutyCycle(int channel) {
  
  std::uint8_t led_on_reg = REG_LED_ON + channel * LED_SHIFT;
  std::uint8_t led_off_reg = REG_LED_OFF + channel * LED_SHIFT;
  
  std::lock_guard<std::mutex> lock {m_mutex};

  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  auto transaction = bus->startTransaction(m_address);
  
  // Read the registers
  std::uint16_t on = bus->readRegister<std::uint16_t>(led_on_reg, true);
  std::uint16_t off = bus->readRegister<std::uint16_t>(led_off_reg, true);
  
  // Check if we have full ON or full OFF enabled
  if (off & CMD_LED_FULL_OFF) {
    return 0;
  }
  if (on & CMD_LED_FULL_ON) {
    return 1;
  }
  
  // Otherwise compute the duty cycle
  on = on & 0x0FFF;
  off = off & 0x0FFF;
  return (off - on) / 4096.;
  
} // end of getDutyCycle

std::unique_ptr<PWM> PCA9685::getAsPWM(int channel) {
  return std::make_unique<LedPwm>(*this, channel);
}

} // end of namespace PiHWCtrl