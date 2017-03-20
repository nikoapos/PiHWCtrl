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
# Simple example of how to use the ADS1115 class. This class can control the
# module ADS1115, which is an analog to digital converter, using the I2C
# protocol.
# 
# The ADS1115 has four analog inputs and it capable to measure the following
# differential voltages:
# 
# - IN 0 - IN 1
# - IN 0 - IN 3
# - IN 1 - IN 3
# - IN 2 - IN 3
# - IN 0 - GND
# - IN 1 - GND
# - IN 2 - GND
# - IN 3 - GND
# 
# Note that all inputs must not be in the range [-0.3V, VDD+0.3V] and that 
# voltages outside this range can harm the device!
# 
# The device itself can measure a single voltage difference at a time and it
# provides a CONTINUOUS mode, which continuously repeats the measurement. This
# class hides from the user the fact of a single measurement per time and it
# provides its own continuous method, which internally switches the multiplexer
# to chose all the input combinations for which the values are requested. At
# the moment the native continuous mode is not accessible.
# 
# Similarly, the ADS1115 class extends the hardware functionality by accepting
# separate gain values for each input, and a special auto-gain mode, which
# switches the gain option to best fit the input.
# 
# Hardware implementation
# -----------------------
# Materials:
#   - An ADS1115 breakout
#   - Two potentiometers
# 
# Connections:
#   - Connect the GND of the ADS1115 to one of the GND pins
#   - Connect the VIN of the ADS1115 to one of the 3.3V pins
#   - Connect the SCL of the ADS1115 to the SCL pin (pin 5 / GPIO 3)
#   - Connect the SDA of the ADS1115 to the SDA pin (pin 3 / GPIO 2)
#   - Connect the ADDR of the ADS1115 to one of the GND pins
#   - Connect the potentiometers one outer pin to one of the GND pins
#   - Connect the potentiometers other outer pin to one of the 3.3V pins
#   - Connect the first potentiometer inner pin to the A0 of the ADS1115
#   - Connect the first potentiometer inner pin to the A3 of the ADS1115
#   - Connect the A2 of the ADS1115 to one of the GND pins
# 
# Execution:
# Run the example. It will start printing the differential voltages of the
# A0 to the ground and the A2 to A3. Try to move the potentiometers from side
# to side to see the values changing. The program will exit after 10 seconds.
# 

import time
import sys
from PiHWCtrl.modules import ADS1115
from PiHWCtrl.modules import ADS1115_factory
from PiHWCtrl.HWInterfaces import ObserverFloat

#
# Create an object for controlling the ADS1115. The first parameter of the
# factory is the pin to which the ADDR pin of the device is connected (this
# determines the address of the device).
#
# The second parameter controls the sampling rate of the device. This is the
# number of measurements performed per second. Higher sampling rates result
# to less accurate, but faster and more power efficient measurements.
#
sensor = ADS1115_factory(ADS1115.GND, ADS1115.DR_32_SPS)
  
#
# When you create the ADS1115 instance, the gains of all differential inputs
# as set to the automatic mode. In this mode the class changes the gain
# to keep the input between 50% and 90% of the full range. You can always
# override (or re-enable) this behavior for a specific input, by calling the
# setGain() method.
#
# Note that here we set the gain to 4, which means that the full range will
# be 1.024V, so the measurements from the second potentiometer bigger than
# this value will be clipped.
#
sensor.setGain(ADS1115.AIN2_AIN3, ADS1115.G_4)
  
#
# After you have setup your ADS1115 class, you can retrieve its measurements
# by one of three ways, as shown below.
#

#
# The first way to get the sensor measurements is to directly call the
# methods of the class. Note that in this case each method call will contact
# the sensor to perform all the required measurements (no caching is
# performed) and it will block until the measurement is done. The blocking
# time depends on the data rate selected.
#
print '\nGetting the measurements using direct function calls:'
in0 = sensor.readConversion(ADS1115.AIN0_GND)
in2 = sensor.readConversion(ADS1115.AIN2_AIN3)
print '   IN2-IN3:', in2
print '       IN0:', in0
  
#
# The second way to get the sensor measurements is by using the methods that
# return AnalogInput implementations. The objects returned by these methods
# are implementing the AnalogInput interface and can be used anywhere an
# AnalogInput is expected. As with the direct method calls, the AnalogInputs
# will trigger the measurements and will block until they are finished.
#
def printAnalogInput(name, analog_input):
    print '%10s: %s' % (name, analog_input.readValue())
    
print '\nGetting the information using the AnalogInputs:'
in0_ai = sensor.conversionAnalogInput(ADS1115.AIN0_GND)
in2_in3_ai = sensor.conversionAnalogInput(ADS1115.AIN2_AIN3)
printAnalogInput('IN0', in0_ai)
printAnalogInput('IN2-IN3', in2_in3_ai)
  
#
# The last way to get the sensor measurements is by registering observers to
# the ADS1115 class and call its start() method. This will put the class in
# continuous measurement mode and it will notify the observers every time a
# measurement is finished. You can always stop the measurements by calling
# the stop() method.
#
# Note that the observer methods will be called in the measurements thread,
# so if their actions are taking too long they will block the measurements.
# If this is the case the observer should delegate the action to a separate
# thread.
#
print '\nGetting the information using observers:'

# This is a helper class that is used to print the messages on the screen.
# Each time the print() method is called it overrides the current line with
# the new values.
class ScreenPrinter:
    def __init__(self):
        self.in0 = 0
        self.in2_in3 = 0
    def message(self):
        print '\r', # Go to the beginning of the line
        in0_str = str(self.in0)
        in0_str = in0_str[:15]
        in2_in3_str = str(self.in2_in3)
        print '  IN0:', '{0: <15}'.format(in0_str), '  IN2-IN3:', in2_in3_str[:15], '         ',
        sys.stdout.flush()
    def setIn0(self, in0):
        self.in0 = in0
        self.message()
    def setIn2In3(self, in2_in3):
        self.in2_in3 = in2_in3
        self.message()

# We create an instance of the ScreenPrinter, which will be updated by the
# observers
printer = ScreenPrinter()

# The PrinterUpdater class is the observer implementation which will be
# registered to the ADS1115. Internally it keeps a function to which it will
# redirect the value when an event occurs.
class PrinterUpdater(ObserverFloat):
    def __init__(self, func):
        ObserverFloat.__init__(self)
        self.func = func
    def event(self, value):
        self.func(value)

#
# We create two instances of the PrinterUpdater and we add them as observers
# to the ADS1115. One that redirects the calls to the setIn0() method
# of the ScreenPrinter and one that redirects the calls to the setIn2In3().
#
in0_observer = PrinterUpdater(lambda x : printer.setIn0(x))
sensor.addConversionObserver(ADS1115.AIN0_GND, in0_observer)
in2_in3_observer = PrinterUpdater(lambda x : printer.setIn2In3(x))
sensor.addConversionObserver(ADS1115.AIN2_AIN3, in2_in3_observer)

#
# We need to call the start() method so the BMP180 will enter the continuous
# measurement mode and it will start notifying the observers. This method
# can also accept a time (in milliseconds) to sleep at each loop after
# performing the measurements. By setting this value and a higher data rate
# at the constructor, you can achieve the same sampling rates, but with
# smaller power consumption (with a penalty at the accuracy).
#
sensor.start()

#
# We wait for 10 seconds to see the results
#
time.sleep(10)

#
# Optionally we an stop the sensor. This will happen anyways when the script exits
#
sensor.stop()

print '\n'

