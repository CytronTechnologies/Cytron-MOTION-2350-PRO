"""
DESCRIPTION:
This example code will uses MOTION 2350 Pro
to read the analog value.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Tutorial link:
https://cytron.io/tutorial/get-started-motion-2350-pro-circuitpython-read-sensor-value
"""
# Import necessary libraries
import board
import time
import analogio

# Define analog pin GP26 used on the board
sensor = analogio.AnalogIn(board.GP26)

while True:
    # Serial print the sensor value every 1 second
    raw_value = sensor.value
    voltage_value = (raw_value * 3.3) / 65536
    print('Raw Value : ', raw_value)
    print('Voltage Value : ', voltage_value)
    print('-------------------------')
    time.sleep(1)
