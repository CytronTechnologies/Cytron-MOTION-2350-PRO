/*
DESCRIPTION:
Getting Started with Motion 2350 Pro using Arduino IDE
This example code will control servo by using USB Controller with USB Host Function in Motion 2350 Pro.

REFERENCE:
2019 Ha Thach for Adafruit Industries
*/

// USBHost is defined in usbh_helper.h
#include "usbh_helper.h"
#include "CytronMotorDriver.h"
#include <Servo.h>

// Configure the servo
Servo myServo1;
const int servoPin1 = 0;

uint16_t input_1 = 0;
uint8_t servo_input = 0;  // Initialize servo input

bool system_ready = false;
unsigned long startup_time;
const unsigned long STARTUP_DELAY = 3000;  // 3 seconds delay

// Function to apply dead zone to inputs
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
  Serial.println("Servo Control System Starting...");

  // Initialize servo
  myServo1.attach(servoPin1);
  myServo1.write(0);  // Set to initial position

  startup_time = millis();
}

void loop() {
  // Check if the system is ready
  if (!system_ready && (millis() - startup_time >= STARTUP_DELAY)) {
    system_ready = true;
    Serial.println("System Ready!");
  }

  // If system is ready, control the servo based on the input_1 value
  if (system_ready) {
    // Servo control logic based on the input_1 value
    if (input_1 == 0x10) {
      myServo1.write(180);
    } else if (input_1 == 0x08) {
      myServo1.write(180);
    } else if (input_1 == 0x02) {
      myServo1.write(0);
    } else if (input_1 == 0x01) {
      myServo1.write(90);
    } else {
      // Default case if no input matches
      myServo1.write(0);
    }
  } else {
    // System not ready, keep servo at initial position
    myServo1.write(0);
  }

  delay(10);  // Small delay to prevent overwhelming the system
}

//------------- Core1 -------------//
void setup1() {
  // Configure pio-usb: defined in usbh_helper.h
  rp2040_configure_pio_usb();

  // Run host stack on controller (rhport) 1
  USBHost.begin(1);
}

void loop1() {
  USBHost.task();
}

// Function to update control data from HID reports
void updateControlData(uint16_t new_input_1, uint8_t new_servo) {
  input_1 = new_input_1;
  servo_input = new_servo;
}

extern "C" {

  // Invoked when device with hid interface is mounted
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

  // Invoked when device with hid interface is unmounted
  void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    Serial.printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
  }

  // Invoked when a report is received from a device via interrupt endpoint
  void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    Serial.printf("HID report: ");
    for (uint16_t i = 0; i < len; i++) {
      Serial.printf("0x%02X ", report[i]);
    }
    Serial.println();

    // Update control data based on report
    updateControlData(report[0], report[3]);

    // Continue to request to receive report
    if (!tuh_hid_receive_report(dev_addr, instance)) {
      Serial.printf("Error: cannot request to receive report\r\n");
    }
  }

}  // extern C
