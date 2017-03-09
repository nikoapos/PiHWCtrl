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
 * @file PiHWCtrl/utils/EventGenerator.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_UTILS_EVENTGENERATOR_H
#define PIHWCTRL_UTILS_EVENTGENERATOR_H

#include <functional>
#include <atomic>
#include <memory>
#include <chrono>
#include <thread>

namespace PiHWCtrl {

template <typename T>
class EventGenerator {
  
public:
  
  using EventFunction = std::function<T()>;
  using NotifyFunction = std::function<void(const T&)>;

  EventGenerator(EventFunction m_event_func, NotifyFunction m_notify_func,
                 std::atomic<bool>& run_flag, unsigned int sleep)
          : m_event_func(m_event_func), m_notify_func(m_notify_func),
            m_run_flag(run_flag), m_sleep(sleep) {
  }
  
  virtual ~EventGenerator() {
    m_run_flag.get() = true;
  }
  
  void operator()() {
    while (m_run_flag.get()) {
      std::this_thread::sleep_for(m_sleep);
      auto value = m_event_func();
      m_notify_func(value);
    }
  }

private:
  
  EventFunction m_event_func;
  NotifyFunction m_notify_func;
  std::reference_wrapper<std::atomic<bool>> m_run_flag;
  std::chrono::milliseconds m_sleep;
  
};

template <typename T>
void startEventGenerator(typename EventGenerator<T>::EventFunction m_event_func,
                         typename EventGenerator<T>::NotifyFunction m_notify_func,
                         std::atomic<bool>& run_flag, unsigned int sleep) {
  EventGenerator<T> task {m_event_func, m_notify_func, run_flag, sleep};
  std::thread t {task};
  t.detach();
}

}

#endif /* PIHWCTRL_UTILS_EVENTGENERATOR_H */

