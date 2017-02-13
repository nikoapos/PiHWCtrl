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
 * @file i2c/I2CBus.h
 * @author nikoapos
 */

#ifndef PIHECTRL_I2CBUS_H
#define PIHECTRL_I2CBUS_H

#include <memory>

namespace PiHWCtrl {

class I2CBus {
  
public:
  
  static std::shared_ptr<I2CBus> getSingleton();
  
  virtual ~I2CBus();
  
private:
  
  I2CBus();
  
  int m_bus_file;

};

} // end of namespace PiHWCtrl

#endif /* PIHECTRL_I2CBUS_H */

