"""
DESCRIPTION:
Demo-Code for MOTION 2350 Pro
This demo code is written in CircuitPython and it serves
as an easy quality check when you first receive the board.

It plays a melody upon power up (slide power switch to ON)
and shows running lights (blue LEDs) at the same time.

Press GP20 button to play a short melody, lights up all 
blue LEDs, motor LEDs.
Press GP21 will execute similar action but in reverse direction.

ROBO PICO also has four DC motors quick test buttons 
built-in. You may press the onboard M1A, M1B, M2A or M2B 
push buttons to run your motors without writing any code.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
"""
import board
import neopixel
import time
import digitalio
import simpleio

INTRO_MELODY_NOTE = [392, 392, 440, 392, 523, 494, 392, 392, 440, 392, 587, 523]
INTRO_MELODY_DURATION = [0.3, 0.1, 0.4, 0.4, 0.4, 0.8, 0.3, 0.1, 0.4, 0.4, 0.4, 0.8]

BTN20_MELODY_NOTE = [494, 466, 440, 415, 392, 370, 349, 330, 311, 294, 277, 262]
BTN20_MELODY_DURATION = [0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.4]

BTN21_MELODY_NOTE = [262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784]
BTN21_MELODY_DURATION = [0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.4]

#Define pin connected to piezo buzzer
PIEZO_PIN = board.GP22

# Define the GPIO pin numbers we'll use
pins_top = [0, 1, 2, 3, 4, 5, 6, 7, 9, 8, 11, 10]
pins_bot = [16, 17, 18, 19, 26, 27, 28, 29, 12, 13, 14, 15]

# Set up each pin as an output
leds_top = [digitalio.DigitalInOut(getattr(board, f'GP{pin}')) for pin in pins_top]
for led in leds_top:
    led.direction = digitalio.Direction.OUTPUT
leds_bot = [digitalio.DigitalInOut(getattr(board, f'GP{pin}')) for pin in pins_bot]
for led in leds_bot:
    led.direction = digitalio.Direction.OUTPUT
    
# Initialize the 2 Neopixel RGB LEDs on pin GP18
pixels = neopixel.NeoPixel(board.GP23, 2)
# Clear Neopixel RGB LED
pixels.fill(0)
# Set pixel brightness
pixels.brightness = 0.3

#Initialize buttons
btn1 = digitalio.DigitalInOut(board.GP20)
btn2 = digitalio.DigitalInOut(board.GP21)
btn1.direction = digitalio.Direction.INPUT
btn2.direction = digitalio.Direction.INPUT
btn1.pull = digitalio.Pull.UP
btn2.pull = digitalio.Pull.UP

#Play melody during start up  
for i in range(len(INTRO_MELODY_DURATION)):
    #The board will not work with 0 frequency, so everytime the frequency is 0, it will rest for a duration of time
    leds_top[i].value = True
    leds_bot[i].value = True
    
    if INTRO_MELODY_NOTE[i] == 0:
        time.sleep(INTRO_MELODY_DURATION[i])
    else:
        # Play melody tones
        simpleio.tone(PIEZO_PIN, INTRO_MELODY_NOTE[i], duration=INTRO_MELODY_DURATION[i])
        
    leds_top[i].value = False
    leds_bot[i].value = False
    
pixels[0] = (100, 0, 0)
pixels[1] = (100, 0, 0)
pixels.show()
time.sleep(0.3)
pixels[0] = (0, 100, 0)
pixels[1] = (0, 100, 0)
pixels.show()
time.sleep(0.3)
pixels[0] = (0, 0, 100)
pixels[1] = (0, 0, 100)
pixels.show()
time.sleep(0.3)
pixels[0] = (0, 0, 0)
pixels[1] = (0, 0, 0)
    
# Main loop
while True:
    # Check button (GP20) is pressed
    if not btn1.value:
        for i in range(len(BTN20_MELODY_DURATION)):
            #The board will not work with 0 frequency, so everytime the frequency is 0, it will rest for a duration of time
            leds_top[i].value = True
            
            if BTN20_MELODY_NOTE[i] == 0:
                time.sleep(BTN20_MELODY_DURATION[i])
            else:
                # Play melody tones
                simpleio.tone(PIEZO_PIN, BTN20_MELODY_NOTE[i], duration=BTN20_MELODY_DURATION[i])
                
            leds_top[i].value = False
            
        pixels[0] = (100, 0, 100)
        pixels.show()
        time.sleep(0.1)
        pixels[0] = (0, 0, 0)
        
        pixels[1] = (100, 0, 100)
        pixels.show()
        time.sleep(0.2)
        pixels[1] = (0, 0, 0)
            
        for i in reversed(range(len(leds_bot))):
            leds_bot[i].value = True
            time.sleep(0.1)
            leds_bot[i].value = False
              
    # Check button (GP21) is pressed
    elif not btn2.value:
        
        for i in range(len(BTN21_MELODY_DURATION)):
            #The board will not work with 0 frequency, so everytime the frequency is 0, it will rest for a duration of time
            leds_bot[i].value = True
            
            if BTN21_MELODY_NOTE[i] == 0:
                time.sleep(BTN21_MELODY_DURATION[i])
            else:
                # Play melody tones
                simpleio.tone(PIEZO_PIN, BTN21_MELODY_NOTE[i], duration=BTN21_MELODY_DURATION[i])
                
            leds_bot[i].value = False
            
        pixels[1] = (100, 0, 100)
        pixels.show()
        time.sleep(0.2)
        pixels[1] = (0, 0, 0)
            
        pixels[0] = (100, 0, 100)
        pixels.show()
        time.sleep(0.2)
        pixels[0] = (0, 0, 0)
        
        for i in reversed(range(len(leds_top))):
            leds_top[i].value = True
            time.sleep(0.1)
            leds_top[i].value = False
