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
 * @file SerializationUtils.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_SERIALIZATIONUTILS_H
#define PIHWCTRL_SERIALIZATIONUTILS_H

#include <cstdint>
#include <algorithm>

namespace PiHWCtrl {

template <typename T, typename I>
void serializeObject(const T& obj, I out_iter) {
  const std::uint8_t* value_ptr = reinterpret_cast<const std::uint8_t*>(&obj);
  std::copy(value_ptr, value_ptr + sizeof(T), out_iter);
}

template <typename T, typename I>
T deserializeObject(I in_iter) {
  T result;
  std::uint8_t* result_ptr = reinterpret_cast<std::uint8_t*>(&result);
  std::copy(in_iter, in_iter + sizeof(T), result_ptr);
  return result;
}

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_SERIALIZATIONUTILS_H */

