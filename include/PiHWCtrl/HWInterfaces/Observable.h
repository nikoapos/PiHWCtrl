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
 * @file Observable.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_HWINTERFACES_OBSERVABLE_H
#define PIHWCTRL_HWINTERFACES_OBSERVABLE_H

#include <vector>
#include <memory>
#include <PiHWCtrl/HWInterfaces/Observer.h>

namespace PiHWCtrl {

/**
 * @class Observable
 * 
 * @brief Interface which can generate events of type T and notify observers
 * 
 * @details
 * Implementations of this interface should call the notifyObservers() method
 * every time they want to generate an event of type T. The Observable has the
 * logic of keeping and notifying the observers already implemented.
 * 
 * @tparam T
 *    The type of the event
 */
template <typename T>
class Observable {
  
public:
  
  virtual ~Observable() = default;
  
  /// Adds an observer, which will be notified for future events
  void addObserver(std::shared_ptr<Observer<T>> observer) {
    m_observers.push_back(observer);
  }
  
protected:
  
  /// Method to be called by the implementations to generate events of type T
  void notifyObservers(const T& value) {
    for (auto& obs : m_observers) {
      obs->event(value);
    }
  }
  
private:
  
  std::vector<std::shared_ptr<Observer<T>>> m_observers;
  
};

}

#endif /* PIHWCTRL_HWINTERFACES_OBSERVABLE_H */

