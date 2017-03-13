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
 * @file StateChangeFilter.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_CONTROLS_STATECHANGEFILTER_H
#define PIHWCTRL_CONTROLS_STATECHANGEFILTER_H

#include <memory>
#include <PiHWCtrl/HWInterfaces/Observer.h>

namespace PiHWCtrl {

/**
 * @class StateChangeFilter
 * 
 * @brief Filter that allow events to pass only when they have new values
 * 
 * @details
 * This control can be used as a decorator of any other control and it will
 * allow the events to pass only when the new value is different than the last
 * event. This type of filter is particularly useful for binary inputs, to treat
 * them as switches.
 * 
 * The type of the even is abstracted as a template parameter, but it has the
 * restriction that it must have a default constructor and copy operator.
 * 
 * @tparam T
 *    The type of the event
 */
template <typename T>
class StateChangeFilter : public Observer<T> {
  
public:
  
  /**
   * @brief Creates a new StateChangeFilter
   * 
   * @param m_observer
   *    The observer to redirect the events when the state changes
   * @param init
   *    The value with which the first event will be compared
   */
  StateChangeFilter(std::shared_ptr<Observer<T>> m_observer, T init={})
          : m_observer(m_observer), m_last(init) {
  }

  /// Default destructor
  virtual ~StateChangeFilter() = default;
  
  /// Forwards the event to the wrapped observer if the value has changed
  void event(const T& value) override {
    if (value != m_last) {
      m_last = value;
      m_observer->event(value);
    }
  }

private:
  
  std::shared_ptr<Observer<T>> m_observer;
  T m_last;
  
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_CONTROLS_STATECHANGEFILTER_H */

