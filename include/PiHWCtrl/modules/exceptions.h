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
 * @file exceptions.h
 * @author nikoapos
 */

#ifndef PIHWCTRL_MODULES_EXCEPTIONS_H
#define PIHWCTRL_MODULES_EXCEPTIONS_H

#include <PiHWCtrl/HWInterfaces/exceptions.h>

namespace PiHWCtrl {

class ModuleAlreadyInUse : public Exception {
public:
  ModuleAlreadyInUse(std::string module_name) : module_name(module_name) {
    appendMessage("Module ");
    appendMessage(module_name);
    appendMessage(" is already in use");
  }
  std::string module_name;
};

class InvalidState : public Exception {
};

} // end of namespace PiHWCtrl

#endif /* PIHWCTRL_MODULES_EXCEPTIONS_H */

