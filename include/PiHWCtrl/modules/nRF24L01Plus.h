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
 * @file nRF24L01Plus.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_NRF24L01PLUS_H
#define PIHWCTRL_NRF24L01PLUS_H

#include <memory>
#include <mutex>
#include <map>
#include <vector>
#include <array>

#include <PiHWCtrl/HWInterfaces/Switch.h>
#include <PiHWCtrl/spi/SPIBus.h>

namespace PiHWCtrl {

class nRF24L01Plus {
  
public:
  
  enum class Channel {
    CH_1,  ///< 2412 MHz
    CH_2,  ///< 2417 MHz
    CH_3,  ///< 2422 MHz
    CH_4,  ///< 2427 MHz
    CH_5,  ///< 2432 MHz
    CH_6,  ///< 2437 MHz
    CH_7,  ///< 2442 MHz
    CH_8,  ///< 2447 MHz
    CH_9,  ///< 2452 MHz
    CH_10, ///< 2457 MHz
    CH_11, ///< 2462 MHz
    CH_12, ///< 2467 MHz
    CH_13  ///< 2472 MHz
  };
  
  enum class Pipe {
      P_0, P_1, P_2, P_3, P_4, P_5
  };
  
  static std::unique_ptr<nRF24L01Plus> factory(
          SPIBus::Device spi_device, std::unique_ptr<PiHWCtrl::Switch> ce,
          Channel channel);
  
  virtual ~nRF24L01Plus();
  
  template <typename T>
  void transmit(const std::array<std::uint8_t, 5>& address,
                const T& message, bool enable_shockburst=false);
  
//  void enablePipe(Pipe pipe, const std::array<std::uint8_t, 5>& address,
//                  int message_size, bool enable_shockburst);
  
private:
  
  nRF24L01Plus(SPIBus::Device spi_device, std::unique_ptr<PiHWCtrl::Switch> ce,
               Channel channel);
  
  mutable std::mutex m_mutex;
  PiHWCtrl::SPIBus m_bus;
  std::unique_ptr<PiHWCtrl::Switch> m_ce;
  std::map<Pipe, bool> m_enable_shockburst {};
  
};

} // end of namespace PiHWCtrl

// Include the implementations of the templated methods
#include <PiHWCtrl/modules/_impl/nRF24L01Plus.icpp>

#endif /* PIHWCTRL_NRF24L01PLUS_H */

