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
# This example controls a row of 5 LED lights, gradually moving the light from
# left to right, using PWM signals. A switch is used as input to show or not
# the light.
# 
# This example must be executed with root privileges (sudo).
# 
# Hardware implementation
# -----------------------
# Materials:
#   - A switch
#   - 5 LEDs (with forward voltage less than 3.3 Volt)
#   - 5 330 ohm resistors (if you want to properly calculate their value see
#     here: http://www.evilmadscientist.com/2012/resistors-for-leds)
# 
# Connections:
#   Switch
#   - Connect the GPIO-21 pin to the one side of the switch
#   - Connect a 3.3 Volt pin to the other side of the switch
#   LEDs
#   - Connect the GPIO-16 pin to the anode of the LED (longer leg)
#   - Connect the cathode of the LED (shorter leg) to the one side of the
#     resistor
#   - Connect the other side of the resistor to the ground (GND)
#   - Repeat the last three steps for the GPIOs 5, 25, 22 and 4
#   - Take care that your LEDs are positioned physically at the correct order:
#         LED:  1  2  3  4  5
#        GPIO: 16  5 25 22  4
#   
# Execution:
# Run the example. When the switch is OFF all LEDs should be OFF. When you turn
# on the switch the LEDs should light from left to right and back in a smooth
# way. Try to play by turning the switch on and off again.
# 

import time
import PiHWCtrl

# Create the object we will use to check if the switch is on or off
on_off = PiHWCtrl.PigpioBinaryInput(21)

# Create a list with the PWM controllers for the GPIOs 16, 5, 25, 22 and 4
leds = [
    PiHWCtrl.PigpioPWM(16),
    PiHWCtrl.PigpioPWM(5),
    PiHWCtrl.PigpioPWM(25),
    PiHWCtrl.PigpioPWM(22),
    PiHWCtrl.PigpioPWM(4)
]

# The i identifies the index of the current LED in the vector
i = 1

# The direction of the movement on the LED vector
direction = 1

# This is the duty cycle (with a value from 0 to 1) for the LED i
duty_cycle = 0

# We change the brightness with steps of 1%
step = 0.01

# Start an infinite loop
while True:
    
    # First turn off all the LEDs
    for led in leds:
        led.setDutyCycle(0)
    
    # Increaes the duty cycle by 1%
    duty_cycle += step
    
    if duty_cycle > 1:
        # If the duty cycle exceeds 100% we move to the next LED (which has duty
        # cycle 0%)
        duty_cycle = 0
        i += direction

        # If we are out of the vector bounds change the direction and move the i
        # at the opposite direction
        if i == -1 or i == len(leds):
            direction = -1 * direction
            i += 2 * direction
    
    # If the switch is on, set the brightness of the LED i to the duty cycle
    # and the brightness of the previous LED to 1 - duty_cycle
    if on_off.isOn():
        leds[i].setDutyCycle(duty_cycle)
        leds[i - direction].setDutyCycle(1 - duty_cycle)
    
    # Sleep for 5 milliseconds
    time.sleep(0.005)