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
 * @file modules/LSM303DLHC.cpp
 * @author nikoapos
 */

#include <cstdint>
#include <map>
#include <PiHWCtrl/i2c/I2CBus.h>
#include <PiHWCtrl/i2c/exceptions.h>
#include <PiHWCtrl/modules/LSM303DLHC.h>



#include <iostream>

namespace PiHWCtrl {

namespace {

constexpr std::uint8_t LSM303DLHC_ACC_ADDRESS = 0x19;
constexpr std::uint8_t LSM303DLHC_MAG_ADDRESS = 0x1E;

// Accelerometer registers
constexpr std::uint8_t REG_CTRL_REG1_A = 0x20;
constexpr std::uint8_t REG_CTRL_REG2_A = 0x21;
constexpr std::uint8_t REG_CTRL_REG3_A = 0x22;
constexpr std::uint8_t REG_CTRL_REG4_A = 0x23;
constexpr std::uint8_t REG_CTRL_REG5_A = 0x24;
constexpr std::uint8_t REG_CTRL_REG6_A = 0x25;
constexpr std::uint8_t REG_REFERENCE_A = 0x26;
constexpr std::uint8_t REG_STATUS_REG_A = 0x27;
constexpr std::uint8_t REG_OUT_X_L_A = 0x28;
constexpr std::uint8_t REG_OUT_X_H_A = 0x29;
constexpr std::uint8_t REG_OUT_Y_L_A = 0x2A;
constexpr std::uint8_t REG_OUT_Y_H_A = 0x2B;
constexpr std::uint8_t REG_OUT_Z_L_A = 0x2C;
constexpr std::uint8_t REG_OUT_Z_H_A = 0x2D;
constexpr std::uint8_t REG_FIFO_CTRL_REG_A = 0x2E;
constexpr std::uint8_t REG_FIFO_SRC_REG_A = 0x2F;
constexpr std::uint8_t REG_INT1_CFG_A = 0x30;
constexpr std::uint8_t REG_INT1_SOURCE_A = 0x31;
constexpr std::uint8_t REG_INT1_THS_A = 0x32;
constexpr std::uint8_t REG_INT1_DURATION_A = 0x33;
constexpr std::uint8_t REG_INT2_CFG_A = 0x34;
constexpr std::uint8_t REG_INT2_SOURCE_A = 0x35;
constexpr std::uint8_t REG_INT2_THS_A = 0x36;
constexpr std::uint8_t REG_INT2_DURATION_A = 0x37;
constexpr std::uint8_t REG_CLICK_CFG_A = 0x38;
constexpr std::uint8_t REG_CLICK_SRC_A = 0x39;
constexpr std::uint8_t REG_CLICK_THS_A = 0x3A;
constexpr std::uint8_t REG_TIME_LIMIT_A = 0x3B;
constexpr std::uint8_t REG_TIME_LATENCY_A = 0x3C;
constexpr std::uint8_t REG_TIME_WINDOW_A = 0x3D;

// Magnetometer registers
constexpr std::uint8_t REG_CRA_REG_M = 0x00;
constexpr std::uint8_t REG_CRB_REG_M = 0x01;
constexpr std::uint8_t REG_MR_REG_M = 0x02;
constexpr std::uint8_t REG_OUT_X_H_M = 0x03;
constexpr std::uint8_t REG_OUT_X_L_M = 0x04;
constexpr std::uint8_t REG_OUT_Z_H_M = 0x05;
constexpr std::uint8_t REG_OUT_Z_L_M = 0x06;
constexpr std::uint8_t REG_OUT_Y_H_M = 0x07;
constexpr std::uint8_t REG_OUT_Y_L_M = 0x08;
constexpr std::uint8_t REG_SR_REG_M = 0x09;
constexpr std::uint8_t REG_IRA_REG_M = 0x0A;
constexpr std::uint8_t REG_IRB_REG_M = 0x0B;
constexpr std::uint8_t REG_IRC_REG_M = 0x0C;
constexpr std::uint8_t REG_TEMP_OUT_H_M = 0x31;
constexpr std::uint8_t REG_TEMP_OUT_L_M = 0x32;


// Magnetometer commands
constexpr std::uint8_t M_TEMP_REGISTER    = REG_CRA_REG_M;
constexpr std::uint8_t M_TEMP_MASK        = 0b10000000;
constexpr std::uint8_t M_TEMP_ENABLE_CMD  = 0b10000000;
constexpr std::uint8_t M_TEMP_DISABLE_CMD = 0b00000000;
constexpr std::uint8_t M_OUTPUT_RATE_REGISTER = REG_CRA_REG_M;
constexpr std::uint8_t M_OUTPUT_RATE_MASK     = 0b00011100;
constexpr std::uint8_t M_OUTPUT_RATE_0_75_CMD = 0b00000000;
constexpr std::uint8_t M_OUTPUT_RATE_1_5_CMD  = 0b00000100;
constexpr std::uint8_t M_OUTPUT_RATE_3_CMD    = 0b00001000;
constexpr std::uint8_t M_OUTPUT_RATE_7_5_CMD  = 0b00001100;
constexpr std::uint8_t M_OUTPUT_RATE_15_CMD   = 0b00010000;
constexpr std::uint8_t M_OUTPUT_RATE_30_CMD   = 0b00010100;
constexpr std::uint8_t M_OUTPUT_RATE_75_CMD   = 0b00011000;
constexpr std::uint8_t M_OUTPUT_RATE_220_CMD  = 0b00011100;
constexpr std::uint8_t M_GAIN_REGISTER = REG_CRB_REG_M;
constexpr std::uint8_t M_GAIN_MASK     = 0b11100000;
constexpr std::uint8_t M_GAIN_1_3_CMD  = 0b00100000;
constexpr std::uint8_t M_GAIN_1_9_CMD  = 0b01000000;
constexpr std::uint8_t M_GAIN_2_5_CMD  = 0b01100000;
constexpr std::uint8_t M_GAIN_4_CMD    = 0b10000000;
constexpr std::uint8_t M_GAIN_4_7_CMD  = 0b10100000;
constexpr std::uint8_t M_GAIN_5_6_CMD  = 0b11000000;
constexpr std::uint8_t M_GAIN_8_1_CMD  = 0b11100000;
constexpr std::uint8_t M_MODE_REGISTER                  = REG_MR_REG_M;
constexpr std::uint8_t M_MODE_MASK                      = 0b00000011;
constexpr std::uint8_t M_MODE_CONTINUOUS_CONVERSION_CMD = 0b00000000;
constexpr std::uint8_t M_MODE_SINGLE_CONVERSION_CMD     = 0b00000001;
constexpr std::uint8_t M_MODE_SLEEP_MODE_CMD            = 0b00000011;


// Magnetometer register bits
constexpr std::uint8_t M_OUTPUT_LOCK_BIT = 0x02;
constexpr std::uint8_t M_DATA_READY_BIT = 0x01;

struct RateInfo {
  RateInfo(float value, std::uint8_t cmd) : value(value), cmd(cmd) { }
  float value;
  std::uint8_t cmd;
};

std::map<LSM303DLHC::MagnetometerOutputRate, RateInfo> mag_rate_info_map {
  {LSM303DLHC::MagnetometerOutputRate::R_0_75_HZ, {0.75, M_OUTPUT_RATE_0_75_CMD}},
  {LSM303DLHC::MagnetometerOutputRate::R_1_5_HZ, {1.5, M_OUTPUT_RATE_1_5_CMD}},
  {LSM303DLHC::MagnetometerOutputRate::R_3_HZ, {3, M_OUTPUT_RATE_3_CMD}},
  {LSM303DLHC::MagnetometerOutputRate::R_7_5_HZ, {7.5, M_OUTPUT_RATE_7_5_CMD}},
  {LSM303DLHC::MagnetometerOutputRate::R_15_HZ, {15, M_OUTPUT_RATE_15_CMD}},
  {LSM303DLHC::MagnetometerOutputRate::R_30_HZ, {30, M_OUTPUT_RATE_30_CMD}},
  {LSM303DLHC::MagnetometerOutputRate::R_75_HZ, {75, M_OUTPUT_RATE_75_CMD}},
  {LSM303DLHC::MagnetometerOutputRate::R_220_HZ, {220, M_OUTPUT_RATE_220_CMD}}
};

struct MagnetometerGainInfo {
  MagnetometerGainInfo(float value, float gain_xy, float gain_z, std::uint8_t cmd,
                       LSM303DLHC::MagnetometerGain previous, LSM303DLHC::MagnetometerGain next)
          : value(value), gain_xy(gain_xy), gain_z(gain_z), cmd(cmd), previous(previous), next(next) { }
  float value;
  float gain_xy;
  float gain_z;
  std::uint8_t cmd;
  LSM303DLHC::MagnetometerGain previous;
  LSM303DLHC::MagnetometerGain next;
};

std::map<LSM303DLHC::MagnetometerGain, MagnetometerGainInfo> mag_gain_info_map {
  {LSM303DLHC::MagnetometerGain::G_1_3, {1.3, 1./1100., 1./980., M_GAIN_1_3_CMD, LSM303DLHC::MagnetometerGain::G_1_3, LSM303DLHC::MagnetometerGain::G_1_9}},
  {LSM303DLHC::MagnetometerGain::G_1_9, {1.9, 1./855., 1./760., M_GAIN_1_9_CMD, LSM303DLHC::MagnetometerGain::G_1_3, LSM303DLHC::MagnetometerGain::G_2_5}},
  {LSM303DLHC::MagnetometerGain::G_2_5, {2.5, 1./670., 1./600., M_GAIN_2_5_CMD, LSM303DLHC::MagnetometerGain::G_1_9, LSM303DLHC::MagnetometerGain::G_4}},
  {LSM303DLHC::MagnetometerGain::G_4, {4., 1./450., 1./400., M_GAIN_4_CMD, LSM303DLHC::MagnetometerGain::G_2_5, LSM303DLHC::MagnetometerGain::G_4_7}},
  {LSM303DLHC::MagnetometerGain::G_4_7, {4.7, 1./400., 1./355., M_GAIN_4_7_CMD, LSM303DLHC::MagnetometerGain::G_4, LSM303DLHC::MagnetometerGain::G_5_6}},
  {LSM303DLHC::MagnetometerGain::G_5_6, {5.6, 1./330., 1./295., M_GAIN_5_6_CMD, LSM303DLHC::MagnetometerGain::G_4_7, LSM303DLHC::MagnetometerGain::G_8_1}},
  {LSM303DLHC::MagnetometerGain::G_8_1, {8.1, 1./230., 1./205., M_GAIN_8_1_CMD, LSM303DLHC::MagnetometerGain::G_5_6, LSM303DLHC::MagnetometerGain::G_8_1}}
};


void setCommand(std::uint8_t reg, std::uint8_t mask, std::uint8_t cmd) {
  auto bus = I2CBus::getSingleton();
  std::uint8_t value = bus->readRegister<std::uint8_t>(reg);
  value = value & !mask;
  value = value | cmd;
  bus->writeRegister(reg, value);
}


void setupMagnetometer(LSM303DLHC::MagnetometerOutputRate rate,
                       LSM303DLHC::MagnetometerGain gain) {
  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Start a transaction that will lock the I2C bus from others until we are done
  auto transaction = bus->startTransaction(LSM303DLHC_MAG_ADDRESS);
  
  // Confirm that we have connected with a LSM303DLHC magnetometer
  if (bus->readRegister<std::uint8_t>(REG_IRA_REG_M) != 0x48 ||
          bus->readRegister<std::uint8_t>(REG_IRB_REG_M) != 0x34 ||
          bus->readRegister<std::uint8_t>(REG_IRC_REG_M) != 0x33) {
    throw I2CWrongModule() << "Attached module is not a LSM303DLHC magnetometer";
  }
  
  // Set the data output rate
  setCommand(M_OUTPUT_RATE_REGISTER, M_OUTPUT_RATE_MASK, mag_rate_info_map.at(rate).cmd);
  
  // Set the gain
  if (gain == LSM303DLHC::MagnetometerGain::AUTO) {
    setCommand(M_GAIN_REGISTER, M_GAIN_MASK, mag_gain_info_map.at(LSM303DLHC::MagnetometerGain::G_1_3).cmd);
  } else {
    setCommand(M_GAIN_REGISTER, M_GAIN_MASK, mag_gain_info_map.at(gain).cmd);
  }
  
  // Enable the magnetometer. At the moment we just use continuous conversion mode.
  setCommand(M_MODE_REGISTER, M_MODE_MASK, M_MODE_CONTINUOUS_CONVERSION_CMD);
  
}

} // end of anonymous namespace

LSM303DLHC::LSM303DLHC(MagnetometerOutputRate mag_rate, MagnetometerGain mag_gain)
        : m_mag_gain(mag_gain), m_mag_auto_gain(MagnetometerGain::G_1_3) {
  
  setupMagnetometer(mag_rate, mag_gain);

}

Vector LSM303DLHC::getMagneticField() {
  // Get the I2C bus
  auto bus = I2CBus::getSingleton();
  
  // Start a transaction that will lock the I2C bus from others until we are done
  auto transaction = bus->startTransaction(LSM303DLHC_MAG_ADDRESS);
  
  // Get the gain info used for this measurement
  auto gain_info = (m_mag_gain == MagnetometerGain::AUTO)
              ? mag_gain_info_map.at(m_mag_auto_gain)
              : mag_gain_info_map.at(m_mag_gain);
  
  // Read the values from the sensor
  std::int16_t x = bus->readRegister<std::int16_t>(REG_OUT_X_H_M);
  std::int16_t y = bus->readRegister<std::int16_t>(REG_OUT_Y_H_M);
  std::int16_t z = bus->readRegister<std::int16_t>(REG_OUT_Z_H_M);
  
  // Calibrate the values
  if (x < m_x_min) m_x_min = x;
  if (x > m_x_max) m_x_max = x;
  x -= (m_x_max + m_x_min) / 2;
  if (y < m_y_min) m_y_min = y;
  if (y > m_y_max) m_y_max = y;
  y -= (m_y_max + m_y_min) / 2;
  y *= (m_x_max - m_x_min) / (m_y_max - m_y_min);
  if (z < m_z_min) m_z_min = z;
  if (z > m_z_max) m_z_max = z;
  z -= (m_z_max + m_z_min) / 2;
  z *= (m_x_max - m_x_min) / (m_z_max - m_z_min);
  
  // Correct for the gain
  auto x_corr = gain_info.gain_xy * x;
  auto y_corr = gain_info.gain_xy * y;
  auto z_corr = gain_info.gain_z * z;
  
  // Check if we need to increase the gain in auto mode
  if (m_mag_gain == MagnetometerGain::AUTO) {
    // Check if we need to increase the gain
    if (m_mag_auto_gain != MagnetometerGain::G_8_1 && 
            (x < -2048 || x > 2047 || y < -2048 || y > 2047 || z < -2048 || z > 2047)) {
      m_mag_auto_gain = mag_gain_info_map.at(m_mag_auto_gain).next;
      setCommand(M_GAIN_REGISTER, M_GAIN_MASK, mag_gain_info_map.at(m_mag_auto_gain).cmd);
    } else if (m_mag_auto_gain != MagnetometerGain::G_1_3 &&
            x > -500 && x < 500 && y > -500 && y < 500 && z > -500 && z < 500) {
      m_mag_auto_gain = mag_gain_info_map.at(m_mag_auto_gain).previous;
      setCommand(M_GAIN_REGISTER, M_GAIN_MASK, mag_gain_info_map.at(m_mag_auto_gain).cmd);
    }
  }
  
  return Vector(x, y, z);
//  return Vector(x_corr, y_corr, z_corr);
}

} // end of namespace PiHWCtrl