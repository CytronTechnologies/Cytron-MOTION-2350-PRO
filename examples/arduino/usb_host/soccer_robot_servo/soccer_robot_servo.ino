/*
DESCRIPTION:
This example code will control omnidirectional robot using MOTION 2350 Pro with mouse.

AUTHOR   : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
2019 Ha Thach for Adafruit Industries
*/

// USBHost is defined in usbh_helper.h
#include "usbh_helper.h"
#include "CytronMotorDriver.h"
#include <Servo.h>

// Configure the servo
Servo myServo1;
Servo myServo2;
const int servoPin1 = 0;
const int servoPin2 = 1;

// Configure the motor driver.
CytronMD motorL(PWM_PWM, 13, 12);
CytronMD motorR(PWM_PWM, 14, 15);

uint16_t input_1 = 0;
uint16_t x_axis = 0;
uint16_t y_axis = 0;
uint8_t servo_input = 0;  // Initialize servo input

bool system_ready = false;
unsigned long startup_time;
const unsigned long STARTUP_DELAY = 3000;  // 5 seconds delay

// Function to map joystick values to motor speed
int mapJoystickToSpeed(uint16_t value) {
  // Map 0-255 to -255 to 255
  return map(value, 0, 255, -255, 255);
}

// Add this function near the top of your file
int applyDeadZone(int value, int deadZone) {
  if (abs(value) < deadZone) {
    return 0;
  }
  return value;
}

//--------------------------------------------------------------------+
// For RP2040 use both core0 for device stack, core1 for host stack
//--------------------------------------------------------------------+

//------------- Core0 -------------//
void setup() {
  Serial.begin(115200);
  Serial.println("Motor and Servo Control System Starting...");

  // Initialize motors to stopped state
  motorL.setSpeed(0);
  motorR.setSpeed(0);

  // Initialize servo
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);
  myServo1.write(0);  // Set to initial position
  myServo2.write(180);

  startup_time = millis();
}

void loop() {
  // Check if startup delay has passed
  if (!system_ready && (millis() - startup_time >= STARTUP_DELAY)) {
    system_ready = true;
    Serial.println("System Ready!");
  }

  if (system_ready) {
    // Map joystick values to motor speeds
    int forward_speed = -mapJoystickToSpeed(y_axis);  // Invert for intuitive control
    int turn_speed = mapJoystickToSpeed(x_axis);

    // Calculate individual motor speeds
    int left_speed = forward_speed + turn_speed;
    int right_speed = forward_speed - turn_speed;

    // Constrain speeds to valid range
    if (input_1 == 0x80) {
      left_speed = constrain(left_speed, -255, 255);
      right_speed = constrain(right_speed, -255, 255);
    } else {
      left_speed = constrain(left_speed, -128, 128);  // when no speed up input received half the speed
      right_speed = constrain(right_speed, -128, 128);
    }

    // Apply dead zone to joystick values
    int deadZone = 5;  // Adjust this value as needed
    left_speed = applyDeadZone(left_speed, deadZone);
    right_speed = applyDeadZone(right_speed, deadZone);

    // Set motor speeds
    motorL.setSpeed(left_speed);
    motorR.setSpeed(right_speed);

    // Constrain speeds to valid range
    if (input_1 == 0x10) {
      myServo1.write(180);
      myServo2.write(0);
    } else if (input_1 == 0x08) {
      myServo1.write(180);
    } else if (input_1 == 0x02) {
      myServo2.write(0);
    } else {
      myServo1.write(0);
      myServo2.write(180);
    }

    // Optional: Print debug information
    Serial.print("Joystick X: ");
    Serial.print(x_axis);
    Serial.print(", Y: ");
    Serial.print(y_axis);
    Serial.print(" | Left Speed: ");
    Serial.print(left_speed);
    Serial.print(", Right Speed: ");
    Serial.println(right_speed);
  } else {
    // System not ready, keep motors stopped and servo at middle position
    motorL.setSpeed(0);
    motorR.setSpeed(0);
    myServo1.write(0);
    myServo2.write(180);
  }

  Serial.flush();
  delay(10);  // Small delay to prevent overwhelming the serial output
}

//------------- Core1 -------------//
void setup1() {
  // configure pio-usb: defined in usbh_helper.h
  rp2040_configure_pio_usb();

  // run host stack on controller (rhport) 1
  // Note: For rp2040 pico-pio-usb, calling USBHost.begin() on core1 will have most of the
  // host bit-banging processing works done in core1 to free up core0 for other works
  USBHost.begin(1);
}

void loop1() {
  USBHost.task();
}

void updateControlData(uint16_t new_input_1, uint16_t new_x, uint16_t new_y, uint8_t new_servo) {
  input_1 = new_input_1;
  x_axis = new_x;
  y_axis = new_y;
  servo_input = new_servo;
}

extern "C" {

  // Invoked when device with hid interface is mounted
  // Report descriptor is also available for use.
  // tuh_hid_parse_report_descriptor() can be used to parse common/simple enough
  // descriptor. Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE,
  // it will be skipped therefore report_desc = NULL, desc_len = 0
  void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
    (void)desc_report;
    (void)desc_len;
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    Serial.printf("HID device address = %d, instance = %d is mounted\r\n", dev_addr, instance);
    Serial.printf("VID = %04x, PID = %04x\r\n", vid, pid);
    if (!tuh_hid_receive_report(dev_addr, instance)) {
      Serial.printf("Error: cannot request to receive report\r\n");
    }
  }

  // Invoked when device with hid interface is un-mounted
  void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    Serial.printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
  }

  // Invoked when received report from device via interrupt endpoint
  void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    Serial.printf("HIDreport : ");
    for (uint16_t i = 0; i < len; i++) {
      Serial.printf("0x%02X ", report[i]);
      updateControlData(report[0], report[3], report[4], report[6]);
    }
    Serial.println();


    // continue to request to receive report
    if (!tuh_hid_receive_report(dev_addr, instance)) {
      Serial.printf("Error: cannot request to receive report\r\n");
    }
  }

}  // extern C