
"""
DESCRIPTION:
This example code will uses MOTION 2350 Pro
to control two stepper motor using 4 channels motor driver.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Tutorial link:
https://cytron.io/tutorial/get-started-motion-2350-pro-circuitpython-dc-motor

Code adapted from Peter Czaja.
"""
# Example to use with Motion 2350 Pro
import board
import digitalio
from adafruit_motor import stepper
from time import sleep

DELAY = 0.005
STEPS = 500

# The Motion 2350 has two sets of four pins suitable for stepper motors:-
# GP9, GP8, GP11 and GP10
# GP12, GP13, GP14 and GP15

# To use with CircuitPython and Motion 2350 Pro
# Set up coils in array :-
coils = (
    digitalio.DigitalInOut(board.GP9),   # M1B 1st pin stepper motor 1
    digitalio.DigitalInOut(board.GP8),   # M1A 2nd pin stepper motor 1
    digitalio.DigitalInOut(board.GP11),  # M2B 3rd pin stepper motor 1
    digitalio.DigitalInOut(board.GP10),  # M2A 4th pin stepper motor 1
    digitalio.DigitalInOut(board.GP12),  # M3A 1st pin stepper motor 2
    digitalio.DigitalInOut(board.GP13),  # M3B 2nd pin stepper motor 2
    digitalio.DigitalInOut(board.GP14),  # M4A 3rd pin stepper motor 2
    digitalio.DigitalInOut(board.GP15),  # M4B 4th pin stepper motor 2
)

for coil in coils:
    coil.direction = digitalio.Direction.OUTPUT

motor1 = stepper.StepperMotor(coils[0], coils[1], coils[2], coils[3], microsteps=None)
motor2 = stepper.StepperMotor(coils[4], coils[5], coils[6], coils[7], microsteps=None)

while True:
    # Forward
    for step in range(STEPS):
        motor1.onestep(direction=stepper.FORWARD)
        motor2.onestep(direction=stepper.FORWARD)
        sleep(DELAY)

    #Backward
    for step in range(STEPS):
        motor1.onestep(direction=stepper.BACKWARD)
        motor2.onestep(direction=stepper.BACKWARD)
        sleep(DELAY)

    # Release motor(s) so that motor is free to spin, also stops using power
    motor1.release()
    motor2.release()
