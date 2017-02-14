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
 * @file controls/MeanFilter.cpp
 * @author nikoapos
 */

#include <PiHWCtrl/controls/MeanFilter.h>

namespace PiHWCtrl {

float MeanFilter::readValue() {
  // Get the new value from the input
  float new_value = m_input->readValue();
  
  // Subtract the contribution of the old value and add the new one
  m_current += (new_value - m_buffer[m_index]) / m_size;
  
  // Replace the value in the buffer
  m_buffer[m_index] = new_value;
  
  // Move the index to the next element
  ++m_index;
  if (m_index == m_size) {
    m_index = 0;
  }
  
  return m_current;
}

} // end of PiHWCtrl namespace