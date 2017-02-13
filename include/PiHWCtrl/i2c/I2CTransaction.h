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
 * @file i2cI2CTransaction.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_I2CTRANSACTION_H
#define PIHWCTRL_I2CTRANSACTION_H

#include <mutex>

namespace PiHWCtrl {

class I2CTransaction {
  
public:
  
  I2CTransaction(std::mutex& mutex) : m_lock(mutex) {
  }
  
  I2CTransaction(I2CTransaction&& other) = default;
  I2CTransaction& operator=(I2CTransaction&& other) = default;
  
  virtual ~I2CTransaction() {
    m_lock.unlock();
  }
  
private:
  
  std::unique_lock<std::mutex> m_lock;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_I2CTRANSACTION_H */

