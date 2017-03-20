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
#include <mutex>
#include <chrono> // for std::chrono_literals
#include <thread>
#include <map>
#include <PiHWCtrl/i2c/I2CBus.h>
#include <PiHWCtrl/i2c/exceptions.h>
#include <PiHWCtrl/utils/FunctionAnalogInput.h>
#include <PiHWCtrl/modules/BMP180.h>
#include <PiHWCtrl/modules/exceptions.h>

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
constexpr std::uint8_t COMMAND_READ_PRESSURE = 0x34;

// Useful delay times
constexpr std::chrono::microseconds RESET_DELAY = 10000us;
constexpr std::chrono::microseconds TEMPERATURE_DELAY = 4500us;

struct ModeInfo {
  ModeInfo(std::uint8_t oss, std::chrono::microseconds wait_time) : oss(oss), wait_time(wait_time) {
  }
  std::uint8_t oss;
  std::chrono::microseconds wait_time;
};

std::map<BMP180::PressureMode, ModeInfo> mode_map {
  {BMP180::PressureMode::ULTRA_LOW_POWER, ModeInfo{0, 4500us}},
  {BMP180::PressureMode::STANDARD, ModeInfo{1, 7500us}},
  {BMP180::PressureMode::HIGH_RESOLUTION, ModeInfo{2, 13500us}},
  {BMP180::PressureMode::ULTRA_HIGH_RESOLUTION, ModeInfo{3, 25500us}}
};

std::mutex instance_exists_mutex;
bool instance_exists = false;

} // end of anonymous namespace

std::unique_ptr<BMP180> BMP180::factory(PressureMode mode, float sea_level_pressure) {
  return std::unique_ptr<BMP180>{new BMP180{mode, sea_level_pressure}};
}


BMP180::BMP180(PressureMode mode, float sea_level_pressure)
        : m_mode(mode), m_sea_level_pressure(sea_level_pressure) {
  
  // Check that there is no other instance controlling the device
  std::unique_lock<std::mutex> lock {instance_exists_mutex};
  if (instance_exists) {
    throw ModuleAlreadyInUse("BMP180");
  } else {
    instance_exists = true;
  }
  lock.unlock();
  
  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Start a transaction that will lock the I2C bus from others until we are done
  auto transaction = bus->startTransaction(BMP180_ADDRESS);
  
  // Confirm that we have connected with a BMP180 chip
  if (bus->readRegister<std::uint8_t>(REGISTER_CHIP_ID) != 0x55) {
    throw I2CWrongModule() << "Attached module is not a BMP180";
  }
  
  // Perform a soft reset to the device and wait until it has finished
  bus->writeRegister(REGISTER_RESET, COMMAND_RESET);
  std::this_thread::sleep_for(RESET_DELAY);
  
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

BMP180::~BMP180() {
  // Stop any threads generating events for the BMP180
  stop();
  // Release the instance_exists flag so new classes can be created
  std::lock_guard<std::mutex> lock {instance_exists_mutex};
  instance_exists = false;
}

std::uint16_t BMP180::readRawTemperature() {
  
  std::lock_guard<std::mutex> lock {m_mutex};
  
  // IF we have done a measurement less than 1 second ago we do not repeat the
  // measurement and just return the same value
  auto now = std::chrono::steady_clock::now();
  if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_temperature_timestamp).count() < 1000) {
    return m_last_temperature;
  }
  m_last_temperature_timestamp = std::chrono::steady_clock::now();

  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Request the measurement by writing to the control register. We do that in a
  // scope so we don't block the I2C bus while waiting for the measurement to
  // be done.
  {
    auto transaction = bus->startTransaction(BMP180_ADDRESS);
    bus->writeRegister(REGISTER_MEASUREMENT_CONTROL, COMMAND_READ_TEMPERATURE);
  }
  
  // Wait until the measurement is completed
  std::this_thread::sleep_for(TEMPERATURE_DELAY);
  
  // Read the uncompensated temperature value from the sensor
  {
    auto transaction = bus->startTransaction(BMP180_ADDRESS);
    m_last_temperature = bus->readRegister<std::uint16_t>(REGISTER_OUT);
  }
  
  return m_last_temperature;
}

std::unique_ptr<AnalogInput<std::uint16_t>> BMP180::rawTemperatureAnalogInput() {
  return std::make_unique<FunctionAnalogInput<std::uint16_t>>(
      [this] () { return this->readRawTemperature(); }
  );
}

void BMP180::addRawTemperatureObserver(std::shared_ptr<Observer<std::uint16_t>> observer) {
  std::lock_guard<std::mutex> lock {m_mutex};
  m_raw_temperature_observable.addObserver(observer);
}

std::int32_t BMP180::computeB5(std::uint16_t ut) {
  std::int32_t x1 = ((ut - m_ac6) * m_ac5) >> 15;
  std::int32_t x2 = (m_mc << 11) / (x1 + m_md);
  return x1 + x2;
}

float BMP180::computeRealTemperature(std::uint16_t ut, std::int32_t b5) {
  long temperature = (b5 + 8) >> 4;
  return temperature * 0.1;
}

float BMP180::readTemperature() {
  std::uint16_t ut = readRawTemperature();
  std::int32_t b5 = computeB5(ut);
  return computeRealTemperature(ut, b5);
}

std::unique_ptr<AnalogInput<float>> BMP180::temperatureAnalogInput() {
  return std::make_unique<FunctionAnalogInput<float>>(
      [this] () { return this->readTemperature(); }
  );
}

void BMP180::addTemperatureObserver(std::shared_ptr<Observer<float>> observer) {
  std::lock_guard<std::mutex> lock {m_mutex};
  m_temperature_observable.addObserver(observer);
}

std::uint32_t BMP180::readRawPressure() {
  std::lock_guard<std::mutex> lock {m_mutex};
  
  // Get the mode info from the map
  auto& mode_info = mode_map.at(m_mode);
  
  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Request the measurement by writing to the control register we do that in a
  // scope so we don't block the I2C bus while waiting for the measurement to
  // be done.
  {
    auto transaction = bus->startTransaction(BMP180_ADDRESS);
    std::uint8_t cmd = COMMAND_READ_PRESSURE + (mode_info.oss << 6);
    bus->writeRegister(REGISTER_MEASUREMENT_CONTROL, cmd);
  }
  
  // Wait for the measurement to be completed
  std::this_thread::sleep_for(mode_info.wait_time);
  
  // Read the uncompensated pressure value from the sensor
  std::uint32_t up = 0;
  {
    auto transaction = bus->startTransaction(BMP180_ADDRESS);
    auto buffer = bus->readRegisterAsArray<3>(REGISTER_OUT);
    up = ((buffer[0] & 0xFF) << 16) | ((buffer[1] & 0xFF) << 8) | (buffer[2] & 0xFF);
    up = up >> (8 - mode_info.oss);
  }
  
  return up;
}

std::unique_ptr<AnalogInput<std::uint32_t>> BMP180::rawPressureAnalogInput() {
  return std::make_unique<FunctionAnalogInput<std::uint32_t>>(
      [this] () { return this->readRawPressure(); }
  );
}

void BMP180::addRawPressureObserver(std::shared_ptr<Observer<std::uint32_t>> observer) {
  std::lock_guard<std::mutex> lock {m_mutex};
  m_raw_pressure_observable.addObserver(observer);
}

float BMP180::computeRealPressure(std::uint16_t ut, std::uint32_t up) {
  // Get the mode info from the map
  auto& mode_info = mode_map.at(m_mode);
  
  std::int32_t pressure = 0;
  std::int32_t b5 = computeB5(ut);
  std::int32_t b6 = b5 - 4000;
  std::int32_t x1 = (m_b2 * (b6 * b6) >> 12) >> 11;
  std::int32_t x2 = (m_ac2 * b6) >> 11;
  std::int32_t x3 = x1 + x2;
  std::int32_t b3 = ((((m_ac1 << 2) + x3) << mode_info.oss) + 2) >> 2;
  x1 = (m_ac3 * b6) >> 13;
  x2 = (m_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  std::uint32_t b4 = (m_ac4 * std::uint32_t(x3 + 32768)) >> 15;
  std::uint32_t b7 = (std::uint32_t(up) - b3) * (50000 >> mode_info.oss);
  if (b7 < 0x80000000) {
    pressure = (b7 * 2) / b4;
  } else {
    pressure = (b7 / b4) * 2;
  }
  x1 = (pressure >> 8) * (pressure >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * pressure) >> 16;
  pressure = pressure + ((x1 + x2 + 3791) >> 4);
  
  return pressure * 0.01;
}

float BMP180::readPressure() {
  std::uint16_t ut = readRawTemperature();
  std::uint32_t up = readRawPressure();
  return computeRealPressure(ut, up);
}

std::unique_ptr<AnalogInput<float>> BMP180::pressureAnalogInput() {
  return std::make_unique<FunctionAnalogInput<float>>(
      [this] () { return this->readPressure(); }
  );
}

void BMP180::addPressureObserver(std::shared_ptr<Observer<float>> observer) {
  std::lock_guard<std::mutex> lock {m_mutex};
  m_pressure_observable.addObserver(observer);
}

float BMP180::computeAltitude(float pressure) {
  float ratio = pressure / m_sea_level_pressure;
  return 44330 * (1 - std::pow(ratio, 1./5.255));
}

float BMP180::readAltitude() {
  float pressure = readPressure();
  return computeAltitude(pressure);
}

std::unique_ptr<AnalogInput<float>> BMP180::altitudeAnalogInput() {
  return std::make_unique<FunctionAnalogInput<float>>(
      [this] () { return this->readAltitude(); }
  );
}

void BMP180::addAltitudeObserver(std::shared_ptr<Observer<float>> observer) {
  std::lock_guard<std::mutex> lock {m_mutex};
  m_altitude_observable.addObserver(observer);
}

float BMP180::getSeaLevelPressure() {
  return m_sea_level_pressure;
}

void BMP180::setSeaLevelPressure(float pressure) {
  m_sea_level_pressure = pressure;
}

float BMP180::calibrateSeaLevelPressure(float altitude) {
  float pressure = readPressure();
  m_sea_level_pressure = pressure / std::pow(1 - altitude / 44330, 5.255);
  return m_sea_level_pressure;
}

void BMP180::start() {
  if (m_observing) {
    throw Exception() << "BMP180 already started";
  }
  m_observing = true;

  auto measurement_task = [this]() {
    while (m_observing) {
      std::uint16_t ut = readRawTemperature();
      std::unique_lock<std::mutex> lock {m_mutex};
      m_raw_temperature_observable.createEvent(ut);
      lock.unlock();
      
      std::int32_t b5 = computeB5(ut);
      float temperature = computeRealTemperature(ut, b5);
      lock.lock();
      m_temperature_observable.createEvent(temperature);
      lock.unlock();
      
      std::uint32_t up = readRawPressure();
      lock.lock();
      m_raw_pressure_observable.createEvent(up);
      lock.unlock();
      
      float pressure = computeRealPressure(ut, up);
      lock.lock();
      m_pressure_observable.createEvent(pressure);
      lock.unlock();
      
      float altitude = computeAltitude(pressure);
      lock.lock();
      m_altitude_observable.createEvent(altitude);
      lock.unlock();
    }
    m_observing = true;
  };
  
  std::thread t {measurement_task};
  t.detach();
}

void BMP180::stop() {
  if (m_observing) {
    // This will trigger the measuring thread to stop
    m_observing = false;
    // We have to wait until the thread signals that it stopped
    while (!m_observing) {
    }
    // Now we can set again the flag to false
    m_observing = false;
  }
}

} // end of namespace PiHWCtrl