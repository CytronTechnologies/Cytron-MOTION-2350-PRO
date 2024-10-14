/*
DESCRIPTION:
This example code will control the Stepper Motor on the MOTION 2350 Pro DC Motor Terminals using Arduino.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor

// initialize the stepper library on DC Motor Driver pins:
Stepper myStepper1(stepsPerRevolution, 9, 8, 11, 10); 
Stepper myStepper2(stepsPerRevolution, 12, 13, 14, 15);

void setup() {
  // set the speed at 60 rpm:
  myStepper1.setSpeed(60);
  myStepper2.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper1.step(stepsPerRevolution);
  myStepper2.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper1.step(-stepsPerRevolution);
  myStepper2.step(-stepsPerRevolution);
  delay(500);
}