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
# Hardware implementation
# -----------------------
# Materials:
#   - A switch
#   - A 10k resistor (used as pull-down)
# 
# Connections:
#   - Connect the GPIO-21 pin to the one side of the switch
#   - Connect a 3.3V pin to the other side of the switch
#   - Connect the one side of the resistor to the GPIO-21
#   - Connect the other side of the resistor to a GND pin
# 
# Execution:
# Run the example. A message will appear indicating if the switch is ON or OFF.
# The program will continue to run for 10 seconds. during this time try to turn
# on and off the switch. You should see messages indicating your actions.
# 

import time
from PiHWCtrl.gpio import GpioBinaryInput
from PiHWCtrl.HWInterfaces import BoolObserver
from PiHWCtrl.controls import BoolStateChangeFilter

# Create an object for getting binary input from the GPIO 21. This object
# implements the PiHWCtrl::BinaryInput interface, so it can be used with any
# function expecting the interface.
bin_in = GpioBinaryInput(21)
  
#
# Getting the state of the pin using method calls
#

# To check if the input pin is on or off you can use the isOn() and isOff()
# methods, which return a boolean
if bin_in.isOn():
    print('GPIO 21 input is ON')
if bin_in.isOff():
    print('GPIO 21 input is OFF')
  
#
# Getting the state of the pin using events
#

# The GpioBinaryInput class implements the BoolObservable interface, which
# means you can add observers that will be notified for the state of the pin.
# The following class is an implementation of such an observer, which will
# print on the screen the message ON or OFF.
class ScreenPrinter(BoolObserver):
    def __init__(self):
        BoolObserver.__init__(self)
    def event(self, value):
        if value:
            print 'ON'
        else:
            print 'OFF'
  
# Create an instance of the observer class, which will be used for printing
# the messages.
screen_printer = ScreenPrinter()

# The GpioBinaryInput will generate events with its state continuously. That
# means that if we add the screen_printer observer directly we will get
# repetitive messages on the screen. To solve this problem you can wrap the
# observer with the BoolStateChangeFilter decorator. This decorator will filter
# out any events for which the state is not changed and will forward only
# the events that the switch turns from OFF to ON and from ON to OFF.
wrapped_observer = BoolStateChangeFilter(screen_printer)

# Add the observer to the input, so it will be notified for its events
bin_in.addObserver(wrapped_observer)

# To have the GpioBinaryInput generating events you have to start it by
# calling the start() method. This method gets a single argument, which is
# the frequency (in milliseconds) in which the events will be created. Here
# we want to get an event every 10 ms.
bin_in.start(10)

# Now we can sleep for 10 seconds before allowing the program to exit, so we
# can turn the switch on and off and see the messages on the screen.
time.sleep(10)