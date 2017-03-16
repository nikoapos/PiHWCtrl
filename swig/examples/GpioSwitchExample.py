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
# Simple example of how to use the PigpioSwitch class. This class can be used to
# control the output of the GPIO pins accessible via the 40 pin interface (GPIOs
# 2-28) as ON (3.3 Volt) or OFF (GND).
# 
# Hardware implementation
# -----------------------
# Materials:
#   - A LED (with forward voltage less than 3.3 Volt)
#   - A 330 ohm resistor (if you want to properly calculate the value see
#     here: http://www.evilmadscientist.com/2012/resistors-for-leds)
# 
# Connections:
#   - Connect the GPIO-26 pin to the anode of the LED (longer leg)
#   - Connect the cathode of the LED (shorter leg) to the one side of the
#     resistor
#   - Connect the other side of the resistor to the ground (GND)
# 
# Execution:
# Run the example. The LED should turn ON and OFF 10 times.
# 

import time
from PiHWCtrl.gpio import GpioSwitch

# Create an object for controlling the GPIO 26 output. This object implements
# the PiHWCtrl::Switch interface, so it can be used with any function
# expecting the interface.
s = GpioSwitch(26)

# Turn the switch ON and OFF 10 times
for i in range(10):
    
    # Turn the LED on
    s.turnOn()
    
    # Wait for half second
    time.sleep(0.5)
    
    # Turn the LED off
    s.turnOff()
    
    # Wait for another half second
    time.sleep(0.5)