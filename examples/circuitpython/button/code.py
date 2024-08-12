"""
DESCRIPTION:
This example code will uses MOTION 2350 Pro
to configure the button to control the LED.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Tutorial link:
https://cytron.io/tutorial/get-started-motion-2350-pro-circuitpython-press-the-buttons
"""
# Import necessary libraries
import board
import digitalio
import time

# Initialize leds on GP0 and GP1 pins
led1 = digitalio.DigitalInOut(board.GP0)
led2 = digitalio.DigitalInOut(board.GP1)

# Set the led pins as output
led1.direction = digitalio.Direction.OUTPUT
led2.direction = digitalio.Direction.OUTPUT

# Initialize buttons on GP20 and GP21
btn1 = digitalio.DigitalInOut(board.GP20)
btn1.direction = digitalio.Direction.INPUT
btn1.pull = digitalio.Pull.UP
btn2 = digitalio.DigitalInOut(board.GP21)
btn2.direction = digitalio.Direction.INPUT
btn2.pull = digitalio.Pull.UP

while True:
    # Check button 1 (GP21)
    if not btn1.value: #Button is pressed 
        # led1 is light up for 0.5s then turned off
        led1.value = True
        time.sleep(0.3)
        led1.value = False
        
    # Check button 2 (GP20)
    if not btn2.value: #Button is pressed
        # led2 is light up for 0.5s then turned off
        led2.value = True
        time.sleep(0.3)
        led2.value = False
