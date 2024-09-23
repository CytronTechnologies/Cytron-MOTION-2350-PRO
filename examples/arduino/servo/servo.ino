/*
DESCRIPTION:
This example code will control the servo on the MOTION 2350 Pro servo port.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;
Servo myservo7;
Servo myservo8;

void setup() {
  myservo1.attach(0);  // attaches the servo on pins to the servo objects
  myservo2.attach(1);
  myservo3.attach(2);
  myservo4.attach(3);
  myservo5.attach(4);
  myservo6.attach(5);
  myservo7.attach(6);
  myservo8.attach(7);
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    myservo3.write(pos);
    myservo4.write(pos);
    myservo5.write(pos);
    myservo6.write(pos);
    myservo7.write(pos);
    myservo8.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    myservo3.write(pos);
    myservo4.write(pos);
    myservo5.write(pos);
    myservo6.write(pos);
    myservo7.write(pos);
    myservo8.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
