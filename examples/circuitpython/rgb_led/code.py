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

# Initialize the 2 RGB LEDs on pin GP23
pixels = neopixel.NeoPixel(board.GP23, 2)

# Clear RGB LEDs
pixels.fill(0)
# Set pixel brightness
pixels.brightness = 0.5

while True:
    # Light up RGB LED with purple colour
    # The sequence of the colour code is (R,G,B) input range is from 0-255 (decimal)
    pixels.fill((200, 0, 200))
    time.sleep(0.5)
    pixels.fill(0)
    time.sleep(0.5)
