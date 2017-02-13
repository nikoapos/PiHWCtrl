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
 * @file modules/BMP180.cpp
 * @author nikoapos
 */

#include <cstdint>
#include <cmath>
#include <chrono> // for std::chrono_literals
#include <thread> // for std::this_thread
#include <PiHWCtrl/modules/BMP180.h>
#include <PiHWCtrl/i2c/I2CBus.h>
#include "PiHWCtrl/i2c/exceptions.h"

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

namespace PiHWCtrl {

namespace {

constexpr std::uint8_t BMP180_ADDRESS = 0x77;

// Registers
constexpr std::uint8_t REGISTER_CHIP_ID = 0xD0;
constexpr std::uint8_t REGISTER_MEASUREMENT_CONTROL = 0xF4;
constexpr std::uint8_t REGISTER_OUT = 0xF6;
constexpr std::uint8_t REGISTER_RESET = 0xE0;
constexpr std::uint8_t REGISTER_CALIBRATION_AC1 = 0xAA;
constexpr std::uint8_t REGISTER_CALIBRATION_AC2 = 0xAC;
constexpr std::uint8_t REGISTER_CALIBRATION_AC3 = 0xAE;
constexpr std::uint8_t REGISTER_CALIBRATION_AC4 = 0xB0;
constexpr std::uint8_t REGISTER_CALIBRATION_AC5 = 0xB2;
constexpr std::uint8_t REGISTER_CALIBRATION_AC6 = 0xB4;
constexpr std::uint8_t REGISTER_CALIBRATION_B1 = 0xB6;
constexpr std::uint8_t REGISTER_CALIBRATION_B2 = 0xB8;
constexpr std::uint8_t REGISTER_CALIBRATION_MB = 0xBA;
constexpr std::uint8_t REGISTER_CALIBRATION_MC = 0xBC;
constexpr std::uint8_t REGISTER_CALIBRATION_MD = 0xBE;

// Useful commands
constexpr std::uint8_t COMMAND_RESET = 0xB6;
constexpr std::uint8_t COMMAND_READ_TEMPERATURE = 0x2E;

}

BMP180::BMP180() {
  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Start a transaction that will lock the I2C bus from others until we are done
  auto transaction = bus->startTransaction(BMP180_ADDRESS);
  
  // Confirm that we have connected with a BMP180 chip
  if (bus->readRegister<std::uint8_t>(REGISTER_CHIP_ID) != 0x55) {
    throw I2CWrongModule() << "Attached module is not a BMP180";
  }
  
  // Do a software reset
  bus->writeRegister(REGISTER_RESET, COMMAND_RESET);
  
  // Read all the calibration registers
  m_ac1 = bus->readRegister<std::int16_t>(REGISTER_CALIBRATION_AC1);
  m_ac2 = bus->readRegister<std::int16_t>(REGISTER_CALIBRATION_AC2);
  m_ac3 = bus->readRegister<std::int16_t>(REGISTER_CALIBRATION_AC3);
  m_ac4 = bus->readRegister<std::uint16_t>(REGISTER_CALIBRATION_AC4);
  m_ac5 = bus->readRegister<std::uint16_t>(REGISTER_CALIBRATION_AC5);
  m_ac6 = bus->readRegister<std::uint16_t>(REGISTER_CALIBRATION_AC6);
  m_b1 = bus->readRegister<std::int16_t>(REGISTER_CALIBRATION_B1);
  m_b2 = bus->readRegister<std::int16_t>(REGISTER_CALIBRATION_B2);
  m_mb = bus->readRegister<std::int16_t>(REGISTER_CALIBRATION_MB);
  m_mc = bus->readRegister<std::int16_t>(REGISTER_CALIBRATION_MC);
  m_md = bus->readRegister<std::int16_t>(REGISTER_CALIBRATION_MD);
  
}

float BMP180::getTemperature() {
  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Request the measurement by writing to the control register we do that in a
  // scope so we don't block the I2C bus while waiting for the measurement to
  // be done.
  {
    auto transaction = bus->startTransaction(BMP180_ADDRESS);
    bus->writeRegister(REGISTER_MEASUREMENT_CONTROL, COMMAND_READ_TEMPERATURE);
  }
  
  // Wait for 5ms so the measurement is completed
  std::this_thread::sleep_for(5ms);
  
  // Read the uncompensated temperature value from the sensor
  std::uint16_t ut = 0;
  {
    auto transaction = bus->startTransaction(BMP180_ADDRESS);
    ut = bus->readRegister<std::uint16_t>(REGISTER_OUT);
  }
  
  // Calculate the true temperature
  long x1 = (ut - m_ac6) * m_ac5 / 32768;
  long x2 = m_mc * 2048 / (x1 + m_md);
  long b5 = x1 + x2;
  long temperature = (b5 + 8) / 16;
  
  return temperature * 0.1;
}

} // end of namespace PiHWCtrl