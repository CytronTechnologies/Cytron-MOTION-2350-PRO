"""
DESCRIPTION:
This example code will uses MOTION 2350 Pro
to display text on OLED Display SSD1315.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Tutorial link:
https://cytron.io/tutorial/get-started-motion-2350-pro-circuitpython-display-text-on-oled
"""
# Import necessary libraries
import board
import busio
import adafruit_ssd1306
import time

# Define the i2c GPIOs on GP16 and GP17
# format: (board.SCL, board.SDA)
i2c = busio.I2C(board.GP17, board.GP16)

# Define the OLED display using the above pins
# format: (width, length, i2c pins)
oled = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c)

while True:
    # Clear the OLED display
    oled.fill(0)

    # Write the data: ('text', x , y, pixel colour)
    # Pixel colour: 0 = false, 1 = true
    oled.text('Hello world!', 0, 0, 1)
    oled.text('Yeah!', 0, 25, 1)

    # Show the written data
    oled.show()
    time.sleep(1)
