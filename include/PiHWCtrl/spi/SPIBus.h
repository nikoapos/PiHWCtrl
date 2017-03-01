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

#include <memory>

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
  
private:
  
  Device m_device;
  int m_bus_file;
  
};

}

#endif /* PIHWCTRL_SPIBUS_H */

