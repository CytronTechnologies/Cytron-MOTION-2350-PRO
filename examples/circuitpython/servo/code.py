"""
DESCRIPTION:
This example code will uses MOTION 2350 Pro
to control the Servos.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Tutorial link:
https://cytron.io/tutorial/get-started-motion-2350-pro-circuitpython-micro-servo-motor
"""
# Import neccessary libraries
import time
import board
import digitalio
import pwmio
from adafruit_motor import servo

# Create a PWMOut object on the control pin.
pwm1 = pwmio.PWMOut(board.GP0, duty_cycle=0, frequency=50)
pwm2 = pwmio.PWMOut(board.GP1, duty_cycle=0, frequency=50)
pwm3 = pwmio.PWMOut(board.GP2, duty_cycle=0, frequency=50)
pwm4 = pwmio.PWMOut(board.GP3, duty_cycle=0, frequency=50)
pwm5 = pwmio.PWMOut(board.GP4, duty_cycle=0, frequency=50)
pwm6 = pwmio.PWMOut(board.GP5, duty_cycle=0, frequency=50)
pwm7 = pwmio.PWMOut(board.GP6, duty_cycle=0, frequency=50)
pwm8 = pwmio.PWMOut(board.GP7, duty_cycle=0, frequency=50)

# You might need to calibrate the min_pulse (pulse at 0 degrees) and max_pulse (pulse at 180 degrees) to get an accurate servo angle.
# The pulse range is 750 - 2250 by default (if not defined).

# Initialize Servo objects.
servo1 = servo.Servo(pwm1, min_pulse=750, max_pulse=2700)
servo2 = servo.Servo(pwm2, min_pulse=750, max_pulse=2700)
servo3 = servo.Servo(pwm3, min_pulse=750, max_pulse=2700)
servo4 = servo.Servo(pwm4, min_pulse=750, max_pulse=2700)
servo5 = servo.Servo(pwm5, min_pulse=750, max_pulse=2700)
servo6 = servo.Servo(pwm6, min_pulse=750, max_pulse=2700)
servo7 = servo.Servo(pwm7, min_pulse=750, max_pulse=2700)
servo8 = servo.Servo(pwm8, min_pulse=750, max_pulse=2700)
angle = 0

while True:
    # Set servo angle
    angle = 0
    # Update servo angles.
    servo1.angle = angle
    servo2.angle = angle
    servo3.angle = angle
    servo4.angle = angle
    servo5.angle = angle
    servo6.angle = angle
    servo7.angle = angle
    servo8.angle = angle
    # Change the angle every 1 second.
    time.sleep(1)

    # Set servo angle
    angle = 90
    # Update servo angles.
    servo1.angle = angle
    servo2.angle = angle
    servo3.angle = angle
    servo4.angle = angle
    servo5.angle = angle
    servo6.angle = angle
    servo7.angle = angle
    servo8.angle = angle
    # Change the angle every 1 second.
    time.sleep(1)
