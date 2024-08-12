"""
DESCRIPTION:
This example code will uses MOTION 2350 Pro
to control 4 channels DC Motor Driver.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Tutorial link:
https://cytron.io/tutorial/get-started-motion-2350-pro-circuitpython-dc-motor
"""
#Import necessary libraries
import time
import board
import pwmio
from adafruit_motor import motor

#Define pins for motor channel
PWM_M1A = board.GP8
PWM_M1B = board.GP9
PWM_M2A = board.GP10
PWM_M2B = board.GP11
PWM_M3A = board.GP12
PWM_M3B = board.GP13
PWM_M4A = board.GP14
PWM_M4B = board.GP15

# DC motor setup
M1A = pwmio.PWMOut(PWM_M1A, frequency=10000)
M1B = pwmio.PWMOut(PWM_M1B, frequency=10000)
M2A = pwmio.PWMOut(PWM_M2A, frequency=10000)
M2B = pwmio.PWMOut(PWM_M2B, frequency=10000)
M3A = pwmio.PWMOut(PWM_M3A, frequency=10000)
M3B = pwmio.PWMOut(PWM_M3B, frequency=10000)
M4A = pwmio.PWMOut(PWM_M4A, frequency=10000)
M4B = pwmio.PWMOut(PWM_M4B, frequency=10000)
motor1 = motor.DCMotor(M1A, M1B)
motor2 = motor.DCMotor(M2A, M2B)
motor3 = motor.DCMotor(M3A, M3B)
motor4 = motor.DCMotor(M4A, M4B)

# Throttle value must be between -1.0 and +1.0 or None (Free-Moving)
print("\nForwards slow")
motor1.throttle = 0.5
motor2.throttle = 0.5
motor3.throttle = 0.5
motor4.throttle = 0.5
time.sleep(1)

print("\nStop")
motor1.throttle = 0
motor2.throttle = 0
motor3.throttle = 0
motor4.throttle = 0
time.sleep(1)

print("\nForwards")
motor1.throttle = 1.0
motor2.throttle = 1.0
motor3.throttle = 1.0
motor4.throttle = 1.0
time.sleep(1)

print("\nStop")
motor1.throttle = 0
motor2.throttle = 0
motor3.throttle = 0
motor4.throttle = 0
time.sleep(1)

print("\nBackwards")
motor1.throttle = -1.0
motor2.throttle = -1.0
motor3.throttle = -1.0
motor4.throttle = -1.0
time.sleep(1)

print("\nStop")
motor1.throttle = 0
motor2.throttle = 0
motor3.throttle = 0
motor4.throttle = 0
time.sleep(1)

print("\nBackwards slow")
motor1.throttle = -0.5
motor2.throttle = -0.5
motor3.throttle = -0.5
motor4.throttle = -0.5
time.sleep(1)

print("\nStop")
motor1.throttle = 0
motor2.throttle = 0
motor3.throttle = 0
motor4.throttle = 0
time.sleep(1)

print("\nSpin freely")
motor1.throttle = None
motor2.throttle = None
motor3.throttle = None
motor4.throttle = None