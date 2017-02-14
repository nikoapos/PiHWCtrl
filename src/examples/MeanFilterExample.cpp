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
 * @file examples/MeanFilterExample.cpp
 * @author nikoapos
 */

#include <iostream>
#include <random>
#include <chrono> // for std::chrono_literals
#include <thread> // for std::this_thread
#include <PiHWCtrl/controls/MeanFilter.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;


class NoisyInput : public PiHWCtrl::AnalogInput {
  
public:
  
  NoisyInput(float mean, float stddev) : m_distribution(mean, stddev) {
  }
  
  float readValue() override {
    return m_distribution(m_generator);
  }
  
private:
  
  std::normal_distribution<float> m_distribution;
  std::mt19937 m_generator {std::random_device{}()};

};


int main() {
  
  auto noisy_input = std::make_shared<NoisyInput>(5, 0.1);
  
  PiHWCtrl::MeanFilter filter {noisy_input, 100};
  
  for (;;) {
    std::cout << noisy_input->readValue() << '\t' << filter.readValue() << '\n';
    std::this_thread::sleep_for(50ms);
  }
  
}