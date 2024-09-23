/*
DESCRIPTION:
This example code will uses MOTION 2350 Pro to read analog value using Arduino.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/

int sensorPin = 29;    // select the input pin for the potentiometer
int raw_value = 0; 
float voltage_value = 0; 
float vin_voltage_value = 0; // solder the jumper at back to measure VIN

void setup() {
  // declare the sensorPin as an INPUT:
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  // enable adc resolution to 12-bit (default 10-bit)
  analogReadResolution(12);
}

void loop() {
  // read the value from the sensor:
  raw_value = analogRead(sensorPin);
  voltage_value = (raw_value * 3.3) / 4095;
  vin_voltage_value = voltage_value * 6.1;

  Serial.print("Raw Value : ");
  Serial.println(raw_value);
  Serial.print("Voltage Value : ");
  Serial.println(voltage_value);
  Serial.print("VIN Voltage Value : ");
  Serial.println(vin_voltage_value);
  Serial.println("---------------------------");
  
  delay(1000);
}
