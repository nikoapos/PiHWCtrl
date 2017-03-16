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
 * @file PiHWCtrl/controls/MeanFilter.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_MEANFILTER_H
#define PIHWCTRL_MEANFILTER_H

#include <vector>
#include <memory>
#include <PiHWCtrl/HWInterfaces/AnalogInput.h>

namespace PiHWCtrl {

class MeanFilter : public AnalogInput<float> {
  
public:
  
  MeanFilter(std::shared_ptr<AnalogInput<float>> input, std::size_t buffer_size)
          : m_input(input), m_size(buffer_size), m_current(input->readValue()),
            m_buffer(buffer_size, m_current) {
  }

  virtual ~MeanFilter() = default;
  
  float readValue() override;

private:
  
  std::shared_ptr<AnalogInput<float>> m_input;
  std::size_t m_size;
  float m_current;
  std::vector<float> m_buffer;
  std::size_t m_index = 0;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_MEANFILTER_H */

