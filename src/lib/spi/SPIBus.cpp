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
 * @file spi/SPIBus.cpp
 * @author nikoapos
 */

#include <string>
#include <map>
#include <cstdint>
#include <fcntl.h> // For open()
#include <unistd.h> // For close()
#include <sys/ioctl.h> // For ioctl()
#include <linux/spi/spidev.h>
#include <PiHWCtrl/utils/GpioManager.h>
#include <PiHWCtrl/spi/SPIBus.h>
#include <PiHWCtrl/spi/exceptions.h>

namespace PiHWCtrl {

namespace {

constexpr int SPI_CE0_GPIO = 8;
constexpr int SPI_CE1_GPIO = 7;
constexpr int SPI_SCLK_GPIO = 11;
constexpr int SPI_MOSI_GPIO = 10;
constexpr int SPI_MISO_GPIO = 9;

constexpr std::uint8_t SPI_MODE = SPI_MODE_0;
constexpr std::uint8_t SPI_BITS_PER_WORD = 8;
constexpr std::uint16_t SPI_DELAY = 0;

struct DeviceInfo {
  std::string filename;
  int gpio;
};

std::map<SPIBus::Device, DeviceInfo> device_info_map {
  {SPIBus::Device::CE_0, {"/dev/spidev0.0", SPI_CE0_GPIO}},
  {SPIBus::Device::CE_1, {"/dev/spidev0.1", SPI_CE1_GPIO}}
};

std::map<SPIBus::Speed, std::uint32_t> speed_info_map {
  {SPIBus::Speed::S_7_629_kHz,    7629},
  {SPIBus::Speed::S_15_2_kHz,    15200},
  {SPIBus::Speed::S_30_5_kHz,    30500},
  {SPIBus::Speed::S_61_kHz,      61000},
  {SPIBus::Speed::S_122_kHz,    122000},
  {SPIBus::Speed::S_244_kHz,    244000},
  {SPIBus::Speed::S_488_kHz,    488000},
  {SPIBus::Speed::S_976_kHz,    976000},
  {SPIBus::Speed::S_1_953_MHz, 1953000},
  {SPIBus::Speed::S_3_9_MHz,   3900000},
  {SPIBus::Speed::S_7_8_MHz,   7800000},
  {SPIBus::Speed::S_15_6_MHz, 15600000},
  {SPIBus::Speed::S_31_2_MHz, 32200000},
  {SPIBus::Speed::S_62_5_MHz, 62500000},
  {SPIBus::Speed::S_125_MHz, 125000000}
};

struct SpiGpioManager {
  
  void reserveGpios() {
    if (m_device_count == 0) {
      GpioManager::getSingleton()->reserveGpio(SPI_SCLK_GPIO);
      GpioManager::getSingleton()->reserveGpio(SPI_MOSI_GPIO);
      GpioManager::getSingleton()->reserveGpio(SPI_MISO_GPIO);
    }
    ++m_device_count;
  }
  
  void releaseGpios() {
    --m_device_count;
    if (m_device_count == 0) {
      GpioManager::getSingleton()->releaseGpio(SPI_SCLK_GPIO);
      GpioManager::getSingleton()->releaseGpio(SPI_MOSI_GPIO);
      GpioManager::getSingleton()->releaseGpio(SPI_MISO_GPIO);
    }
  }
  
  int m_device_count = 0;
  
};

SpiGpioManager spi_gpio_manager {};

} // end of anonymous namespace

SPIBus::SPIBus(Device device, Speed speed) : m_device(device) {
  // Reserve the common SPI GPIOs
  spi_gpio_manager.reserveGpios();
  
  // Reserve the SPI_CE GPIO for the given device
  auto& device_info = device_info_map.at(m_device);
  GpioManager::getSingleton()->reserveGpio(device_info.gpio);
  
  // Open the file for using the bus
  m_bus_file = open(device_info.filename.c_str(), O_RDWR);
  if (m_bus_file < 0) {
    throw SPIBusOpenFailure(device_info.filename);
  }
  
  // Set the SPI mode
  if (ioctl(m_bus_file, SPI_IOC_WR_MODE, &SPI_MODE) != 0) {
    throw SPIModeException(SPI_MODE);
  }
  
  // Set the SPI bits per word
  if (ioctl(m_bus_file, SPI_IOC_WR_BITS_PER_WORD, &SPI_BITS_PER_WORD) != 0) {
    throw SPIBitsPerWordException(SPI_BITS_PER_WORD);
  }
  
  // Set the max speed of the bus
  if (ioctl(m_bus_file, SPI_IOC_WR_MAX_SPEED_HZ, &(speed_info_map.at(speed))) != 0) {
    throw SPISpeedException(speed_info_map.at(speed));
  }
  
  // Setup the required info for the spi_ioc_transfer object that will be used
  // for the transfers
  m_delay_usecs = SPI_DELAY;
  m_speed_hz = speed_info_map.at(speed);
  m_bits_per_word = SPI_BITS_PER_WORD;
  
}

SPIBus::~SPIBus() {
  // Release the common SPI GPIOs
  spi_gpio_manager.releaseGpios();
  
  // Release the SPI_CE GPIO so other objects can use it
  auto& device_info = device_info_map.at(m_device);
  GpioManager::getSingleton()->releaseGpio(device_info.gpio);
  
  // Close the bus file
  close(m_bus_file);
}

} // end of namespace PiHWCtrl