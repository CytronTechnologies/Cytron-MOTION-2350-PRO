"""
DESCRIPTION:
This example code will uses MOTION 2350 Pro
to light up the RGB LEDs.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Tutorial link:
https://cytron.io/tutorial/get-started-motion-2350-pro-circuitpyhton-lighting-up-the-rgb-led
"""
# Import necessary libraries
import board
import neopixel
import time

# Initialize Neopixel RGB LED on pin GP23
pixel = neopixel.NeoPixel(board.GP23, 2)

# Clear Neopixel RGB LED
pixel.fill(0)
# Set pixel brightness
pixel.brightness = 0.5

# Define each colour codes in RGB decimal format
RED = (255, 0, 0)
ORANGE = (255,180,0)
YELLOW = (80, 80, 0)
GREEN = (0, 255, 0)
CYAN = (0, 255, 255)
BLUE = (0, 0, 255)
PURPLE = (180, 0, 255)
WHITE = (100, 100, 100)

# Group all the colours in an array
colour = [RED,ORANGE,YELLOW,GREEN,CYAN,BLUE,PURPLE,WHITE]

while True:
  # Light up the neopixel RGB LED and change the colour every 0.15s
  for i in range(len(colour)):
    pixel.fill(colour[i])
    time.sleep(0.5)
    