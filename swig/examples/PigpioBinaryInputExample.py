# 
# Copyright (C) 2017 nikoapos
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
 
# 
# Description
# -----------
# 
# Simple example of how to use the PigpioBinaryInput class. This class can be
# used to read binary input from one of the GPIO pins accessible via the 40 pin
# interface (GPIOs 2-28). It interprets the input as following:
# 
# - ON: 3.3 Volt connected to the pin
# - OFF: GND connected to the pin or the pin is open circuited
# 
# Under the hood, this class uses the pigpio library, so any program using it
# must be executed with root privileges (sudo).
# 
# Hardware implementation
# -----------------------
# Materials:
#   - A switch
# 
# Connections:
#   - Connect the GPIO-21 pin to the one side of the switch
#   - Connect a 3.3 Volt pin to the other side of the switch
# 
# Execution:
# Turn the switch off and run the example. Then turn the switch on and re-run
# it. You should see terminal messages indicating the position of the switch.
# 

import PiHWCtrl

# Create an object for getting binary input from the GPIO 21
bin_in = PiHWCtrl.PigpioBinaryInput(21)

# To check if the input pin is on or off you can use the isOn() and isOff()
# methods, which return a boolean
if bin_in.isOn():
    print('GPIO 21 input is ON')
if bin_in.isOff():
    print('GPIO 21 input is OFF')