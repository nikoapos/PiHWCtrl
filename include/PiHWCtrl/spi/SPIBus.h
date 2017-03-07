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
 * @file SPIBus.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_SPIBUS_H
#define PIHWCTRL_SPIBUS_H

#include <cstdint>
#include <array>
#include <sys/ioctl.h> // For ioctl()
#include <linux/spi/spidev.h>
#include <PiHWCtrl/spi/exceptions.h>

namespace PiHWCtrl {

class SPIBus {
  
public:
  
  enum class Device {
    CE_0, CE_1
  };
  
  enum class Speed {
    S_7_629_kHz, S_15_2_kHz, S_30_5_kHz, S_61_kHz, S_122_kHz, S_244_kHz,
    S_488_kHz, S_976_kHz, S_1_953_MHz, S_3_9_MHz, S_7_8_MHz, S_15_6_MHz,
    S_31_2_MHz, S_62_5_MHz, S_125_MHz
  };
  
  SPIBus(Device device, Speed speed=Speed::S_488_kHz);
  
  virtual ~SPIBus();
  
  template <std::size_t Size>
  std::array<std::uint8_t, Size> transferArray(std::array<std::uint8_t, Size> tx) {
    
    std::array<std::uint8_t, Size> rx;
    
    spi_ioc_transfer tr;
    tr.tx_buf = std::uint64_t(tx.data());
    tr.rx_buf = std::uint64_t(rx.data());
    tr.len = Size;
    tr.delay_usecs = m_delay_usecs;
    tr.speed_hz = m_speed_hz;
    tr.bits_per_word = m_bits_per_word;
    tr.cs_change = 0;
    tr.tx_nbits=0;
    tr.rx_nbits=0;
    tr.pad=0;

    if (ioctl(m_bus_file, SPI_IOC_MESSAGE(1), &tr) != Size) {
      throw SPITransferException();
    }
    
    return rx;
  }
  
private:
  
  Device m_device;
  int m_bus_file;
  std::uint16_t m_delay_usecs;
  std::uint32_t m_speed_hz;
  std::uint8_t m_bits_per_word;
  
};

}

#endif /* PIHWCTRL_SPIBUS_H */

