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
 * @file nRF24L01Plus_constants.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_NRF24L01PLUS_CONSTANTS_H
#define PIHWCTRL_NRF24L01PLUS_CONSTANTS_H

#include <PiHWCtrl/modules/nRF24L01Plus.h>

namespace PiHWCtrl {
namespace _nRF24L01Plus_impl {

// SPI commands
constexpr std::uint8_t SPI_CMD_R_REGISTER = 0x00;
constexpr std::uint8_t SPI_CMD_W_REGISTER = 0x20;
constexpr std::uint8_t SPI_CMD_R_RX_PAYLOAD = 0x61;
constexpr std::uint8_t SPI_CMD_W_TX_PAYLOAD = 0xA0;
constexpr std::uint8_t SPI_CMD_FLUSH_TX = 0xE1;
constexpr std::uint8_t SPI_CMD_FLUSH_RX = 0xE2;
constexpr std::uint8_t SPI_CMD_REUSE_TX_PL = 0xE3;
constexpr std::uint8_t SPI_CMD_R_RX_PL_WID = 0x60;
constexpr std::uint8_t SPI_CMD_W_ACK_PAYLOAD = 0xA8;
constexpr std::uint8_t SPI_CMD_W_TX_PAYLOAD_NO_ACK = 0xB0;
constexpr std::uint8_t SPI_CMD_NOP = 0xFF;

// Registers
constexpr std::uint8_t REG_CONFIG = 0x00;
constexpr std::uint8_t REG_EN_AA = 0x01;
constexpr std::uint8_t REG_EN_RXADDR = 0x02;
constexpr std::uint8_t REG_SETUP_AW = 0x03;
constexpr std::uint8_t REG_SETUP_RETR = 0x04;
constexpr std::uint8_t REG_RF_CH = 0x05;
constexpr std::uint8_t REG_RF_SETUP = 0x06;
constexpr std::uint8_t REG_STATUS = 0x07;
constexpr std::uint8_t REG_OBSERVE_TX = 0x08;
constexpr std::uint8_t REG_RPD = 0x09;
constexpr std::uint8_t REG_ADDR_P0 = 0x0A;
constexpr std::uint8_t REG_ADDR_P1 = 0x0B;
constexpr std::uint8_t REG_ADDR_P2 = 0x0C;
constexpr std::uint8_t REG_ADDR_P3 = 0x0D;
constexpr std::uint8_t REG_ADDR_P4 = 0x0E;
constexpr std::uint8_t REG_ADDR_P5 = 0x0F;
constexpr std::uint8_t REG_TX_ADDR = 0x10;
constexpr std::uint8_t REG_RX_PW_P0 = 0x11;
constexpr std::uint8_t REG_RX_PW_P1 = 0x12;
constexpr std::uint8_t REG_RX_PW_P2 = 0x13;
constexpr std::uint8_t REG_RX_PW_P3 = 0x14;
constexpr std::uint8_t REG_RX_PW_P4 = 0x15;
constexpr std::uint8_t REG_RX_PW_P5 = 0x16;
constexpr std::uint8_t REG_FIFO_STATUS = 0x17;
constexpr std::uint8_t REG_DYNPD = 0x1C;
constexpr std::uint8_t REG_FEATURE = 0x1D;

// Commands
constexpr std::uint8_t CMD_PWR_UP = 0x02;
constexpr std::uint8_t CMD_PRIM_RX = 0x01;
constexpr std::uint8_t CMD_EN_CRC = 0x08;


struct ChannelInfo {
  ChannelInfo(std::uint8_t rf_ch) : rf_ch(rf_ch) { }
  std::uint8_t rf_ch;
};

std::map<nRF24L01Plus::Channel, ChannelInfo> channel_map {
  //                            frequency
  {nRF24L01Plus::Channel::CH_1,  {12}},
  {nRF24L01Plus::Channel::CH_2,  {17}},
  {nRF24L01Plus::Channel::CH_3,  {22}},
  {nRF24L01Plus::Channel::CH_4,  {27}},
  {nRF24L01Plus::Channel::CH_5,  {32}},
  {nRF24L01Plus::Channel::CH_6,  {37}},
  {nRF24L01Plus::Channel::CH_7,  {42}},
  {nRF24L01Plus::Channel::CH_8,  {47}},
  {nRF24L01Plus::Channel::CH_9,  {52}},
  {nRF24L01Plus::Channel::CH_10, {57}},
  {nRF24L01Plus::Channel::CH_11, {62}},
  {nRF24L01Plus::Channel::CH_12, {67}},
  {nRF24L01Plus::Channel::CH_13, {72}}
};


struct PipeInfo {
  PipeInfo(std::uint8_t enable_cmd) : enable_cmd(enable_cmd) { }
  std::uint8_t enable_cmd;
};

std::map<nRF24L01Plus::Pipe, PipeInfo> pipe_map {
  //                       enable_cmd
  {nRF24L01Plus::Pipe::P_0, {0x01}},
  {nRF24L01Plus::Pipe::P_1, {0x02}},
  {nRF24L01Plus::Pipe::P_2, {0x04}},
  {nRF24L01Plus::Pipe::P_3, {0x08}},
  {nRF24L01Plus::Pipe::P_5, {0x10}},
  {nRF24L01Plus::Pipe::P_0, {0X20}}
};

}
}

#endif /* PIHWCTRL_NRF24L01PLUS_CONSTANTS_H */

