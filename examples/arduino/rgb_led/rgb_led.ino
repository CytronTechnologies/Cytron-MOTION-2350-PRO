/*
DESCRIPTION:
This example code will light up neopixel rgb led on MOTION 2350 Pro using Arduino.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Adafruit_NeoPixel library link:
https://github.com/adafruit/Adafruit_NeoPixel/tree/master
*/
#include <Adafruit_NeoPixel.h>

const int neoPin = 23;

const int numPixels = 2;

// Initialize the 2 Neopixel RGB LEDs on pin GP18
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

void setup() {
  // Initialize the 2 Neopixel RGB LEDs on pin GP18
  pixels.begin(); 
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();  // Send the updated pixel colors to the hardware.
}

void loop() {
  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  pixels.setPixelColor(0, pixels.Color(200, 0, 200));
  pixels.setPixelColor(1, pixels.Color(200, 0, 200));
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(1000);

  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(1000);
}
