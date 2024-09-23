/*
DESCRIPTION:
This example code will light up LEDs by pressing buttons on MOTION 2350 Pro using Arduino.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/

const int ledPin1 = 0;
const int ledPin2 = 1;
const int btn1 = 20;
const int btn2 = 21;

void setup() {
  // initialize leds on GP0 and GP1 pins as output.
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
}

void loop() {
  // Check button 1 (GP20) is pressed
  if (!digitalRead(btn1)) {
    // led GP0 is light up for 0.5s then turned off.
    digitalWrite(ledPin1, HIGH);
    delay(500);
    digitalWrite(ledPin1, LOW);
  }

  // Check button 1 (GP21) is pressed
  if (!digitalRead(btn2)) {
    // led GP1 is light up for 0.5s then turned off.
    digitalWrite(ledPin2, HIGH);
    delay(500);
    digitalWrite(ledPin2, LOW);
  }
}
