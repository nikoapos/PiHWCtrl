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
 * @file PiHWCtrl/modules/LSM303DLHC.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_LSM303DLHC_H
#define PIHWCTRL_LSM303DLHC_H

#include <PiHWCtrl/HWInterfaces/Vector.h>

namespace PiHWCtrl {

class LSM303DLHC {
  
public:
  
  enum class MagnetometerOutputRate {
    R_0_75_HZ, R_1_5_HZ, R_3_HZ, R_7_5_HZ, R_15_HZ, R_30_HZ, R_75_HZ, R_220_HZ
  };
  
  enum class MagnetometerGain {
    G_1_3, G_1_9, G_2_5, G_4, G_4_7, G_5_6, G_8_1, AUTO
  };
  
  LSM303DLHC(MagnetometerOutputRate mag_rate=MagnetometerOutputRate::R_15_HZ,
             MagnetometerGain mag_gain=MagnetometerGain::AUTO);
  
  virtual ~LSM303DLHC() = default;
  
  Vector getMagneticField();
  
private:
  
  MagnetometerGain m_mag_gain;
  MagnetometerGain m_mag_auto_gain;
  std::int16_t m_x_min = -646;
  std::int16_t m_x_max = 561;
  std::int16_t m_y_min = -682;
  std::int16_t m_y_max = 501;
  std::int16_t m_z_min = -443;
  std::int16_t m_z_max = 430;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_LSM303DLHC_H */

