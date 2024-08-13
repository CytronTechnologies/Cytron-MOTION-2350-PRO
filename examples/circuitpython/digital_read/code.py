"""
DESCRIPTION:
This example code will uses MOTION 2350 Pro
to read the ultrasonic sensor.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Tutorial link:
https://cytron.io/tutorial/get-started-motion-2350-pro-circuitpython-read-sensor-value
"""
# Import necessary libraries
import time
import board
import digitalio
import adafruit_hcsr04

# Define pin GP26 and GP27 used on the board
sonar = adafruit_hcsr04.HCSR04(trigger_pin=board.GP17, echo_pin=board.GP16)

while True:
    Distance = sonar.distance
    print('Distance : ', Distance, 'cm')
    time.sleep(1)
  
