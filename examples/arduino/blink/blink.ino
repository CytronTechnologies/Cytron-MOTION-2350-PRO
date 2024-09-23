/*
DESCRIPTION:
This example code will light up LEDs on MOTION 2350 Pro using Arduino.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/

const int ledPin1 = 0;
const int ledPin2 = 1;

void setup() {
  // initialize leds on GP0 and GP1 pins as output.
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  // led GP0 is light up for 0.5s then turned off.
  digitalWrite(ledPin1, HIGH);   
  delay(500);                      
  digitalWrite(ledPin1, LOW);  
  delay(500);                    

  // led GP1 is light up for 0.5s then turned off.
  digitalWrite(ledPin2, HIGH);  
  delay(500);      
  digitalWrite(ledPin2, LOW);  
  delay(500);                  
}
