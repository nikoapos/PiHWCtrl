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
# Simple example of how to use the BMP180 class. This class can control the
# module BMP180, which is a pressure and temperature sensor controlled using
# the I2C protocol.
# 
# The BMP180 class provides the measured pressure and temperature values and
# also provides the altitude, which is computed based on the sea level
# pressure, which can either be given by the user or it can be calibrated using
# a known altitude.
# 
# Hardware implementation
# -----------------------
# Materials:
#   - A BMP180 breakout
# 
# Connections:
#   - Connect the GND of the BMP180 to one of the GND pins
#   - Connect the VIN of the BMP180 to one of the 3.3V pins
#   - Connect the SCL of the BMP180 to the SCL pin (pin 5 / GPIO 3)
#   - Connect the SDA of the BMP180 to the SDA pin (pin 3 / GPIO 2)
# 
# Execution:
# Run the example. You will see messages with the current temperature, pressure
# and altitude for 10 seconds.
# 

import time
import sys
from PiHWCtrl.modules import BMP180
from PiHWCtrl.modules import BMP180_factory
from PiHWCtrl.HWInterfaces import FloatObserver

#
# Create an object for controlling the BMP180. The first parameter of the
# factory is the pressure sampling accuracy, which can be one of the
# following:
#        ULTRA_LOW_POWER : Conversion time  4.5ms. RMS 0.06hPa - 0.5m
#               STANDARD : Conversion time  7.5ms, RMS 0.05hPa - 0.4m (default)
#        HIGH_RESOLUTION : Conversion time 13.5ms, RMS 0.04hPa - 0.3m
#  ULTRA_HIGH_RESOLUTION : Conversion time 25.5ms, RMS 0.03hPa - 0.25m
#
# The second parameter is the value of the sea level pressure, which will be
# used for computing the altitude.
#
sensor = BMP180_factory(BMP180.ULTRA_HIGH_RESOLUTION)

#
# If you know your exact altitude, you can use it to calibrate the sea level
# pressure used for the altitude measurements. The BMP180 class provides a
# method for retrieving the computed sea level pressure, so you can even use it
# for implementing a weather station.
#
sensor.calibrateSeaLevelPressure(486)
sea_pres = sensor.getSeaLevelPressure()
print 'Assuming you are at 486 m altitude, the sea level pressure is', sea_pres
  
#
# You can always manually update the sea level pressure after you create the
# BMP180 instance. This is useful, for example, if you periodically check a
# weather station to get an up to date value.
#
sensor.setSeaLevelPressure(1030)
  
#
# After you have setup your BMP180 class, you can retrieve its measurements
# by one of three ways, as shown below.
#

#
# The first way to get the sensor measurements is to directly call the
# methods of the class. Note that in this case each method call will contact
# the sensor to perform all the required measurements (no caching is
# performed) and it will block until the measurement is done. The blocking
# time depends on the method and the pressure sampling accuracy selected.
#
print '\nGetting the information using direct function calls:'
raw_temp = sensor.readRawTemperature()
temp = sensor.readTemperature()
raw_pres = sensor.readRawPressure()
pres = sensor.readPressure()
alt = sensor.readAltitude()
print '     Raw temperature:', raw_temp
print '         Temperature:', temp, 'C'
print '        Raw pressure:', raw_pres
print '            Pressure:', pres, 'hPa'
print '            Altitude:', alt, 'm'
  
#
# The second way to get the sensor measurements is by using the methods that
# return AnalogInput implementations. The objects returned by these methods
# are implementing the AnalogInput interface and can be used anywhere an
# AnalogInput is expected. As with the direct method calls, the AnalogInputs
# will trigger the measurements and will block until they are finished.
#
def printAnalogInput(name, analog_input):
    print '%20s: %s' % (name, analog_input.readValue())
    
print '\nGetting the information using the AnalogInputs:'
raw_temp_ai = sensor.rawTemperatureAnalogInput()
printAnalogInput('Raw temperature', raw_temp_ai)
temp_ai = sensor.temperatureAnalogInput()
printAnalogInput('Temperature', temp_ai)
raw_pres_ai = sensor.rawPressureAnalogInput()
printAnalogInput('Raw pressure', raw_pres_ai)
pres_ai = sensor.pressureAnalogInput()
printAnalogInput('Pressure', pres_ai)
alt_ai = sensor.altitudeAnalogInput()
printAnalogInput('Altitude', alt_ai)
  
#
# The last way to get the sensor measurements is by registering observers to
# the BMP180 class and call its start() method. This will put the class in
# continuous measurement mode and it will notify the observers every time a
# measurement is finished. You can always stop the measurements by calling
# the stop method.
#
# Note that the observer methods will be called in the measurements thread,
# so if their actions are taking too long they will block the measurements.
# If this is the case the observer should delegate the action to a separate
# thread.
#
print '\nGetting the information using observers:'
  
# This is a helper class that is used to print the messages on the screen.
# Each time the message() method is called it overrides the current line with
# the new values. For simplicity we only show the temperature and the
# altitude.
class ScreenPrinter:
    def __init__(self):
        self.temp = 0
        self.alt = 0
    def message(self):
        print '\r', # Go to the beginning of the line
#        print '  Temperature:', self.temp, '  Altitude:', self.alt,
#        print '  Temperature: %20s  Altitude $20s' % (self.temp, self.alt)
        temp_str = str(self.temp)
        temp_str = temp_str[:4]
        alt_str = str(self.alt)
        print '  Temperature:', '{0: <5}'.format(temp_str), '  Altitude:', alt_str[:6], '         ',
        sys.stdout.flush()
    def setTemperature(self, temp):
        self.temp = temp
        self.message()
    def setAltitude(self, alt):
        self.alt = alt
        self.message()

# We create an instance of the ScreenPrinter, which will be updated by the
# observers
printer = ScreenPrinter()
  
# The PrinterUpdater class is the observer implementation which will be
# registered to the BMP180. Internally it keeps a function to which it will
# redirect the value when an event occurs.
class PrinterUpdater(FloatObserver):
    def __init__(self, func):
        FloatObserver.__init__(self)
        self.func = func
    def event(self, value):
        self.func(value)
  
#
# We create two instances of the PrinterUpdater and we add them as observers
# to the BMP180. One that redirects the calls to the setTemperature() method
# of the ScreenPrinter and one that redirects the calls to the setAltitude().
#
temp_observer = PrinterUpdater(lambda x : printer.setTemperature(x))
sensor.addTemperatureObserver(temp_observer)
alt_observer = PrinterUpdater(lambda x : printer.setAltitude(x))
sensor.addAltitudeObserver(alt_observer)

#
# We need to call the start() method so the BMP180 will enter the continuous
# measurement mode and it will start notifying the observers
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
