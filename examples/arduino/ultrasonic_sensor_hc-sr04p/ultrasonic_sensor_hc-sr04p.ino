/*
DESCRIPTION:
This example code will read distance from Ultrasonic Sensor HC-SR04P using Arduino.

CONNECTION:
Robo Pico Grove 4 : HC-SR04P
GP16              - Echo
GP17              - Trig

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Code adapted from www.HowToMechatronics.com:
https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
*/

// defines pins numbers
const int echoPin = 16;
const int trigPin = 17;

// defines variables
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
}