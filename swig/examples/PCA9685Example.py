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
# Simple example of how to use the PCA9685 class. This class can be used to
# control the PCA9685 module, which provides 16 PWM channels. This example uses
# a single PWM channel to control the brightness of a LED. As the LED is not
# draining too much current, the 5V provided by the Raspberry Pi itself is used
# to power it up.
# 
# Hardware implementation
# -----------------------
# Materials:
#   - A PCA9685 breakout
#   - A LED
#   - A 330 ohm resistor (if you want to properly calculate the value see
#     here: http://www.evilmadscientist.com/2012/resistors-for-leds)
# 
# Connections:
#   - Connect the GND of the PCA9685 to one of the GND pins
#   - Connect all the address pins of the PCA9685 to the GND (address 0x40)
#   - Connect the VDD of the PCA9685 to one of the 3.3V pins
#   - Connect the SCL of the ADS1115 to the SCL pin (pin 5 / GPIO 3)
#   - Connect the SDA of the ADS1115 to the SDA pin (pin 3 / GPIO 2)
#   - Connect the V+ of the PCA9685 to one of the 5V pins
#   - Connect the one side of the resistor to the PWM of the channel 12 of the
#     PCA9685
#   - Connect the cathode of the LED (shorter leg) to the other side of the
#     resistor
#   - Connect the other side of the LED to the ground (GND)
# 
# Execution:
# Run the example. The LED should gradually change from fully off to full on
# and back 10 times.
# 

import time
from PiHWCtrl.modules import PCA9685_factory

# 
# Create an object for controlling the PCA9685. The first parameter is the
# address of the device, which can be changed by using the address pins. The
# second parameter, which is optional, sets the frequency of the PWM pulses.
# The default (used here) is 200Hz.
#
controller = PCA9685_factory(0x40)

# We are going to use the channel 12
channel = 12

# We will change the brightness of the LED in steps of 1%
step = 0.01

counter = 0
while (counter < 10):
    # We compute the new duty cycle by getting the current value (using the
    # getDutyCycle() method) and adding the step
    new_duty_cycle = controller.getDutyCycle(channel) + step

    # If the new duty cycle is outside the range [0%, 100%] we change the
    # direction and we increase the counter
    if new_duty_cycle < 0 or new_duty_cycle > 1:
        step *= -1
        new_duty_cycle += 2 * step
        counter += 1

    # Set the new duty cycle of the PWM
    controller.setDutyCycle(channel, new_duty_cycle)

    # Wait for 10 milliseconds
    time.sleep(0.01)

#
# Alternatively, you can use the getAsPWM() method to retrieve a PWM instance
# for controlling a specific channel. This object can be used anywhere where
# the PWM interface is expected.
#
def handlePWM(pwm, duty_cycle):
    pwm.setDutyCycle(duty_cycle)
    print "The PWM duty cycle is:", pwm.getDutyCycle()

pwm = controller.getAsPWM(channel)
handlePWM(pwm, 0.7)