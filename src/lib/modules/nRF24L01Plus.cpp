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
 * @file nRF24L01Plus.cpp
 * @author nikoapos
 */

#include <cstdint>
#include <chrono> // for std::chrono_literals
#include <thread>
#include <map>

#include <PiHWCtrl/modules/exceptions.h>
#include <PiHWCtrl/modules/nRF24L01Plus.h>
#include <PiHWCtrl/modules/_impl/nRF24L01Plus_constants.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

using namespace PiHWCtrl::_nRF24L01Plus_impl;

namespace PiHWCtrl {


std::unique_ptr<nRF24L01Plus> nRF24L01Plus::factory(
        SPIBus::Device spi_device, std::unique_ptr<PiHWCtrl::Switch> ce,
        Channel channel) {
  return std::unique_ptr<nRF24L01Plus>{new nRF24L01Plus{spi_device, std::move(ce), channel}};
}


nRF24L01Plus::nRF24L01Plus(SPIBus::Device spi_device,
                           std::unique_ptr<PiHWCtrl::Switch> ce,
                           Channel channel)
        : m_bus(spi_device), m_ce(std::move(ce)) {
  
  // Be sure the CE is low so we will enter in standby mode
  m_ce->turnOff();
  
  // First we sleep for 100ms which is the required time from turning on till
  // Power Down state
  std::this_thread::sleep_for(100ms);
  
  // Set the frequency of the device
  m_bus.writeCommand(SPI_CMD_W_REGISTER | REG_RF_CH, channel_map.at(channel).rf_ch);
  
  // By default we disable all pipes
  m_bus.writeCommand(SPI_CMD_W_REGISTER | REG_EN_RXADDR, 0x00);
  
  // Now we can power up the device. At the same time we enable the CRC, using
  // a single byte, and we set the type of the device as RX
  std::uint8_t conf = CMD_PWR_UP | CMD_PRIM_RX | CMD_EN_CRC;
  m_bus.writeCommand(SPI_CMD_W_REGISTER | REG_CONFIG, conf);
  
  // Wait until the device is started
  std::this_thread::sleep_for(5ms);
  
}


nRF24L01Plus::~nRF24L01Plus() {
  
  // Be sure the CE is low
  m_ce->turnOff();
  
  // Power off the device
  auto conf = m_bus.readCommand<std::uint8_t>(SPI_CMD_R_REGISTER | REG_CONFIG);
  conf = conf & (!CMD_PWR_UP);
  m_bus.writeCommand(SPI_CMD_W_REGISTER | REG_CONFIG, conf);

}


} // end of namespace PiHWCtrl