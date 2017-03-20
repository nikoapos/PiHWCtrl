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
 * @file modules/ADS1115.cpp
 * @author nikoapos
 */

#include <cstdint>
#include <mutex>
#include <map>
#include <chrono> // for std::chrono_literals
#include <thread>

#include <PiHWCtrl/i2c/I2CBus.h>
#include <PiHWCtrl/i2c/exceptions.h>
#include <PiHWCtrl/modules/ADS1115.h>
#include <PiHWCtrl/modules/exceptions.h>
#include <PiHWCtrl/utils/FunctionAnalogInput.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

namespace PiHWCtrl {

namespace {

constexpr std::uint8_t ADDRESS_GND = 0x48;
constexpr std::uint8_t ADDRESS_VDD = 0x49;
constexpr std::uint8_t ADDRESS_SDA = 0x4A;
constexpr std::uint8_t ADDRESS_SCL = 0x4B;

// Registers
constexpr std::uint8_t REG_CONVERSION = 0x00;
constexpr std::uint8_t REG_CONFIG = 0x01;
constexpr std::uint8_t REG_LO_THRESH = 0x02;
constexpr std::uint8_t REG_HI_THRESH = 0x03;

// Commands for conversion
constexpr std::uint16_t CMD_CONV_MASK = 0x8000;
constexpr std::uint16_t CMD_CONV_BEGIN_SINGLE = 0x8000;

// Commands for multiplexer
constexpr std::uint16_t CMD_MUX_MASK = 0x7000;
constexpr std::uint16_t CMD_MUX_0_1 = 0x0000;
constexpr std::uint16_t CMD_MUX_0_3 = 0x1000;
constexpr std::uint16_t CMD_MUX_1_3 = 0x2000;
constexpr std::uint16_t CMD_MUX_2_3 = 0x3000;
constexpr std::uint16_t CMD_MUX_0_GND = 0x4000;
constexpr std::uint16_t CMD_MUX_1_GND = 0x5000;
constexpr std::uint16_t CMD_MUX_2_GND = 0x6000;
constexpr std::uint16_t CMD_MUX_3_GND = 0x7000;

// Commands for gain
constexpr std::uint16_t CMD_GAIN_MASK = 0x0E00;
constexpr std::uint16_t CMD_GAIN_2_3 = 0x0000;
constexpr std::uint16_t CMD_GAIN_1 = 0x0200;
constexpr std::uint16_t CMD_GAIN_2 = 0x0400;
constexpr std::uint16_t CMD_GAIN_4 = 0x0600;
constexpr std::uint16_t CMD_GAIN_8 = 0x0800;
constexpr std::uint16_t CMD_GAIN_16 = 0x0A00;

// Commands for operating mode
constexpr std::uint16_t CMD_MODE_MASK = 0x0100;
constexpr std::uint16_t CMD_MODE_CONTINUOUS = 0x0000;
constexpr std::uint16_t CMD_MODE_SINGLE_SHOT = 0x0100;

// Commands for data rate
constexpr std::uint16_t CMD_DATA_RATE_MASK = 0x00E0;
constexpr std::uint16_t CMD_DATA_RATE_8_SPS = 0x0000;
constexpr std::uint16_t CMD_DATA_RATE_16_SPS = 0x0020;
constexpr std::uint16_t CMD_DATA_RATE_32_SPS = 0x0040;
constexpr std::uint16_t CMD_DATA_RATE_64_SPS = 0x0060;
constexpr std::uint16_t CMD_DATA_RATE_128_SPS = 0x0080;
constexpr std::uint16_t CMD_DATA_RATE_250_SPS = 0x00A0;
constexpr std::uint16_t CMD_DATA_RATE_475_SPS = 0x00C0;
constexpr std::uint16_t CMD_DATA_RATE_860_SPS = 0x00E0;

// Commands for comparator
constexpr std::uint16_t CMD_COMP_MODE_MASK = 0x0010;
constexpr std::uint16_t CMD_COMP_MODE_TRADITIONAL = 0x0000;
constexpr std::uint16_t CMD_COMP_MODE_WINDOW = 0x0010;
constexpr std::uint16_t CMD_COMP_POL_ACTIVE_MASK = 0x0008;
constexpr std::uint16_t CMD_COMP_POL_ACTIVE_LOW = 0x0000;
constexpr std::uint16_t CMD_COMP_POL_ACTIVE_HIGH = 0x0008;
constexpr std::uint16_t CMD_COMP_LAT_MASK = 0x0004;
constexpr std::uint16_t CMD_COMP_LAT_DISABLE = 0x0000;
constexpr std::uint16_t CMD_COMP_LAT_ENABLE = 0x0004;
constexpr std::uint16_t CMD_COMP_QUE_MASK = 0x0003;
constexpr std::uint16_t CMD_COMP_QUE_1 = 0x0000;
constexpr std::uint16_t CMD_COMP_QUE_2 = 0x0001;
constexpr std::uint16_t CMD_COMP_QUE_4 = 0x0002;
constexpr std::uint16_t CMD_COMP_QUE_DISABLE = 0x0003;


struct AddressPinInfo {
  AddressPinInfo(std::string name, std::uint8_t address) : name(name), address(address) { }
  std::string name;
  std::uint8_t address;
};

std::map<ADS1115::AddressPin, AddressPinInfo> address_map {
  //                                        Name   Address
  {ADS1115::AddressPin::GND, AddressPinInfo{"GND", ADDRESS_GND}},
  {ADS1115::AddressPin::VDD, AddressPinInfo{"VDD", ADDRESS_VDD}},
  {ADS1115::AddressPin::SDA, AddressPinInfo{"SDA", ADDRESS_SDA}},
  {ADS1115::AddressPin::SCL, AddressPinInfo{"SCL", ADDRESS_SCL}}
};

struct GainInfo {
  GainInfo(float gain, float full_scale, std::uint16_t command)
          : gain(gain), full_scale(full_scale), command(command) { }
  float gain;
  float full_scale;
  std::uint16_t command;
};

std::map<ADS1115::Gain, GainInfo> gain_map {
  //                              gain   full-scale  command 
  {ADS1115::Gain::G_2_3, GainInfo{2./3., 6.144,      CMD_GAIN_2_3}},
  {ADS1115::Gain::G_1,   GainInfo{   1., 4.096,      CMD_GAIN_1}},
  {ADS1115::Gain::G_2,   GainInfo{   2., 2.048,      CMD_GAIN_2}},
  {ADS1115::Gain::G_4,   GainInfo{   4., 1.024,      CMD_GAIN_4}},
  {ADS1115::Gain::G_8,   GainInfo{   8., 0.512,      CMD_GAIN_8}},
  {ADS1115::Gain::G_16,  GainInfo{  16., 0.256,      CMD_GAIN_16}}
};

struct InputInfo {
  InputInfo(std::uint16_t command) : command(command) { }
  std::uint16_t command;
};

std::map<ADS1115::Input, InputInfo> input_map {
  //                                    command
  {ADS1115::Input::AIN0_AIN1, InputInfo{CMD_MUX_0_1}}, 
  {ADS1115::Input::AIN0_AIN3, InputInfo{CMD_MUX_0_3}}, 
  {ADS1115::Input::AIN1_AIN3, InputInfo{CMD_MUX_1_3}}, 
  {ADS1115::Input::AIN2_AIN3, InputInfo{CMD_MUX_2_3}}, 
  {ADS1115::Input::AIN0_GND,  InputInfo{CMD_MUX_0_GND}}, 
  {ADS1115::Input::AIN1_GND,  InputInfo{CMD_MUX_1_GND}}, 
  {ADS1115::Input::AIN2_GND,  InputInfo{CMD_MUX_2_GND}}, 
  {ADS1115::Input::AIN3_GND,  InputInfo{CMD_MUX_3_GND}}
};

struct DataRateInfo {
  DataRateInfo(int rate, std::uint16_t command)
          : rate(rate), wait_time(1000000/rate), command(command) { }
  int rate;
  std::chrono::microseconds wait_time;
  std::uint16_t command;
};

std::map<ADS1115::DataRate, DataRateInfo> data_rate_map {
  //                                           rate   command
  {ADS1115::DataRate::DR_8_SPS,   DataRateInfo{8,     CMD_DATA_RATE_8_SPS}},
  {ADS1115::DataRate::DR_16_SPS,  DataRateInfo{16,    CMD_DATA_RATE_16_SPS}},
  {ADS1115::DataRate::DR_32_SPS,  DataRateInfo{32,    CMD_DATA_RATE_32_SPS}},
  {ADS1115::DataRate::DR_64_SPS,  DataRateInfo{64,    CMD_DATA_RATE_64_SPS}},
  {ADS1115::DataRate::DR_128_SPS, DataRateInfo{128,   CMD_DATA_RATE_128_SPS}},
  {ADS1115::DataRate::DR_250_SPS, DataRateInfo{250,   CMD_DATA_RATE_250_SPS}},
  {ADS1115::DataRate::DR_475_SPS, DataRateInfo{475,   CMD_DATA_RATE_475_SPS}},
  {ADS1115::DataRate::DR_860_SPS, DataRateInfo{860,   CMD_DATA_RATE_860_SPS}}
};

std::mutex instance_exists_mutex;

std::map<std::uint8_t, bool> instance_exist_map {
  {ADDRESS_GND, false},
  {ADDRESS_VDD, false},
  {ADDRESS_SDA, false},
  {ADDRESS_SCL, false}
};

std::uint16_t addCmd(std::uint16_t reg, std::uint16_t mask, std::uint16_t command) {
  return ((reg & ~mask) & 0xFFFF) | command;
}

} // end of anonymous namespace

std::unique_ptr<ADS1115> ADS1115::factory(AddressPin addr, Gain gain, DataRate data_rate) {
  return std::unique_ptr<ADS1115>{new ADS1115{addr, gain, data_rate}};
}

ADS1115::ADS1115(AddressPin addr, Gain gain, DataRate data_rate)
        : m_addr(address_map.at(addr).address), m_gain(gain), m_data_rate(data_rate) {
  
  // Check that there is no other instance controlling the device
  std::unique_lock<std::mutex> lock {instance_exists_mutex};
  if (instance_exist_map.at(m_addr)) {
    throw ModuleAlreadyInUse("ADS1115-"+address_map.at(addr).name);
  } else {
    instance_exist_map.at(m_addr) = true;
  }
  lock.unlock();
  
  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Start a transaction that will lock the I2C bus from others until we are done
  auto transaction = bus->startTransaction(m_addr);
  
  // We set the gain to the requested value
  std::uint16_t cmd = 0x0000;
  cmd |= gain_map.at(m_gain).command;
  
  // We set the mode to single shot
  cmd |= CMD_MODE_SINGLE_SHOT;
  m_mode = Mode::SINGLE_SHOT;
  
  // We set the data rate to the requested value
  cmd |= data_rate_map.at(m_data_rate).command;
  
  // Set everything else to the default values
  cmd |= CMD_MUX_0_1;
  cmd |= CMD_COMP_MODE_TRADITIONAL;
  cmd |= CMD_COMP_POL_ACTIVE_LOW;
  cmd |= CMD_COMP_LAT_DISABLE;
  cmd |= CMD_COMP_QUE_DISABLE;
  bus->writeRegister(REG_CONFIG, cmd, true);

}

ADS1115::~ADS1115() {
  // Stop any threads generating events for the ADS1115
  stop();
  // Release the instance_exists flag so new classes can be created
  std::lock_guard<std::mutex> lock {instance_exists_mutex};
  instance_exist_map.at(m_addr) = false;
}

float ADS1115::readConversion(Input input) {
  // First check that we are in single shot mode
  if (m_mode == Mode::CONTINUOUS) {
    throw InvalidState() << "ADS1115: cannot call readConversion() when in CONTINUOUS mode";
  }
  
  std::lock_guard<std::mutex> lock {m_mutex};

  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Send the command for triggering the measurement. We do this in a scope to
  // don't block the I2C bus while waiting for the measurement to be done.
  {
    auto transaction = bus->startTransaction(m_addr);
    // Read the current config register
    std::uint16_t cmd = bus->readRegister<std::uint16_t>(REG_CONFIG);
    // Update the input to read
    cmd = addCmd(cmd, CMD_MUX_MASK, input_map.at(input).command);
    // Set the bit for triggering the single conversion
    cmd = addCmd(cmd, CMD_CONV_MASK, CMD_CONV_BEGIN_SINGLE);
    // Send the command
    bus->writeRegister(REG_CONFIG, cmd, true);
  }
  
  // Now we sleep according the data rate, until the conversion finishes
  std::this_thread::sleep_for(data_rate_map.at(m_data_rate).wait_time);
  
  // Wait until the conversion bit of the config register indicates that the
  // measurement is done
  {
    auto transaction = bus->startTransaction(m_addr);
    for (std::uint16_t conf = 0x0000; conf & CMD_CONV_MASK == 0; conf=bus->readRegister<std::int16_t>(REG_CONFIG)) {
    }
  }
  
  // Read the conversion register
  std::int16_t value;
  {
    auto transaction = bus->startTransaction(m_addr);
    value = bus->readRegister<std::int16_t>(REG_CONVERSION);
  }
  
  // Convert the value to voltage, according the gain and the full scale
  float voltage = (gain_map.at(m_gain).full_scale / 0x7FFF) * value;
  
  return voltage;
  
} // end of readConversion()

std::unique_ptr<AnalogInput<float>> ADS1115::conversionAnalogInput(Input input) {
  return std::make_unique<FunctionAnalogInput<float>>(
    [this, input] () { return this->readConversion(input); }
  );
}

void ADS1115::addConversionObserver(Input input, std::shared_ptr<Observer<float>> observer) {
  std::lock_guard<std::mutex> lock {m_mutex};
  // We use the [] and not the at() method so the observable will be created the
  // first time an observer is registered for this input
  m_observable_map[input].addObserver(observer);
}

void ADS1115::start() {
  if (m_observing) {
    throw Exception() << "ADS1115 already started";
  }
  m_observing = true;

  auto measurement_task = [this]() {
    while (m_observing) {
      for (auto& pair : m_observable_map) {
        float value = readConversion(pair.first);
        std::unique_lock<std::mutex> lock {m_mutex};
        pair.second.createEvent(value);
        lock.unlock();
      }
    }
    m_observing = true;
  };
  
  std::thread t {measurement_task};
  t.detach();
}

void ADS1115::stop() {
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