/*
DESCRIPTION:
This example code will control omnidirectional robot using MOTION 2350 Pro with mouse.

AUTHOR   : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
2019 Ha Thach for Adafruit Industries
*/

#include "usbh_helper.h"
#include "CytronMotorDriver.h"

// Motor configuration
CytronMD rightFrontMotor(PWM_PWM, 8, 9);   // Right front motor
CytronMD rightBackMotor(PWM_PWM, 10, 11);  // Right back motor
CytronMD leftFrontMotor(PWM_PWM, 12, 13);  // Left front motor
CytronMD leftBackMotor(PWM_PWM, 14, 15);   // Left back motor

// Mouse movement values
int8_t mouse_x = 0;
int8_t mouse_y = 0;
uint8_t mouse_click = 0;
int8_t mouse_scroll = 0;

bool system_ready = false;
unsigned long startup_time;
const unsigned long STARTUP_DELAY = 3000; // 3 seconds delay

// New variables for speed multiplier
float speed_multiplier = 1.0;
const float MULTIPLIER_INCREMENT = 0.5;
const float MAX_MULTIPLIER = 10.0;
const float MIN_MULTIPLIER = 0.5;
uint8_t previous_mouse_click = 0;

// Function to map mouse movement to motor speed
int mapMouseToSpeed(int8_t value) {
  // Apply exponential mapping for more precise control
  float normalized = value / 127.0f;  // Normalize to -1 to 1
  // float exponential = pow(abs(normalized), 2) * 255;  // Square for exponential curve, scale to 255
  // return (int)(exponential * (normalized >= 0 ? 1 : -1));
  return normalized * 255;
}

// New variables for scroll handling
int accumulated_scroll = 0;
const int SCROLL_THRESHOLD = 1;  // Adjust this value to change sensitivity
const int MAX_ROTATION_SPEED = 128;  // Maximum rotation speed

// Function to handle scroll input
int handleScrollInput(int8_t scroll_input) {
  accumulated_scroll += scroll_input;
  
  if (abs(accumulated_scroll) >= SCROLL_THRESHOLD) {
    int rotation = (accumulated_scroll > 0) ? MAX_ROTATION_SPEED : -MAX_ROTATION_SPEED;
    accumulated_scroll = 0;  // Reset accumulated scroll
    return rotation;
  }
  
  return 0;  // No rotation if threshold not met
}


//------------- Core0 -------------//
void setup() {
  Serial.begin(115200);
  Serial.println("Omnidirectional Car Control System Starting...");
  
  // Initialize motors to stopped state
  rightFrontMotor.setSpeed(0);
  rightBackMotor.setSpeed(0);
  leftFrontMotor.setSpeed(0);
  leftBackMotor.setSpeed(0);

  startup_time = millis();
}

void loop() {
  // Check if startup delay has passed
  if (!system_ready && (millis() - startup_time >= STARTUP_DELAY)) {
    system_ready = true;
    Serial.println("System Ready!");
  }

  if (system_ready) {
    // Handle mouse clicks for speed multiplier
    if ((mouse_click & 0x01) && !(previous_mouse_click & 0x01)) {  // Left click (new press)
      speed_multiplier = min(speed_multiplier + MULTIPLIER_INCREMENT, MAX_MULTIPLIER);
    } else if ((mouse_click & 0x02) && !(previous_mouse_click & 0x02)) {  // Right click (new press)
      speed_multiplier = max(speed_multiplier - MULTIPLIER_INCREMENT, MIN_MULTIPLIER);
    }
    previous_mouse_click = mouse_click;

    // Map mouse movement to motor speeds
    int forward_speed = -mapMouseToSpeed(mouse_y);  // Invert for intuitive control
    int strafe_speed = mapMouseToSpeed(mouse_x);
    int rotation_speed = handleScrollInput(mouse_scroll);  // Enhanced scroll handling

    // Apply speed multiplier
    forward_speed *= speed_multiplier;
    strafe_speed *= speed_multiplier;

    // Calculate individual motor speeds for omnidirectional movement including rotation
    int rf_speed = forward_speed - strafe_speed - rotation_speed;
    int rb_speed = forward_speed + strafe_speed - rotation_speed;
    int lf_speed = forward_speed + strafe_speed + rotation_speed;
    int lb_speed = forward_speed - strafe_speed + rotation_speed;

    // Constrain speeds to valid range
    rf_speed = constrain(rf_speed, -255, 255);
    rb_speed = constrain(rb_speed, -255, 255);
    lf_speed = constrain(lf_speed, -255, 255);
    lb_speed = constrain(lb_speed, -255, 255);

    // Set motor speeds
    rightFrontMotor.setSpeed(rf_speed);
    rightBackMotor.setSpeed(rb_speed);
    leftFrontMotor.setSpeed(lf_speed);
    leftBackMotor.setSpeed(lb_speed);

    // Print debug information
    Serial.print("Mouse - X: ");
    Serial.print(mouse_x);
    Serial.print(", Y: ");
    Serial.print(mouse_y);
    Serial.print(", Click: ");
    Serial.print(mouse_click, BIN);
    Serial.print(", Scroll: ");
    Serial.print(mouse_scroll);
    Serial.print(", Rotation: ");
    Serial.print(rotation_speed);
    Serial.print(", Multiplier: ");
    Serial.print(speed_multiplier);
    Serial.print(" | Motors - RF: ");
    Serial.print(rf_speed);
    Serial.print(", RB: ");
    Serial.print(rb_speed);
    Serial.print(", LF: ");
    Serial.print(lf_speed);
    Serial.print(", LB: ");
    Serial.println(lb_speed);

    // Reset mouse movement to stop when no new input
    mouse_x = 0;
    mouse_y = 0;
    mouse_scroll = 0;
  } else {
    // System not ready, keep motors stopped
    rightFrontMotor.setSpeed(0);
    rightBackMotor.setSpeed(0);
    leftFrontMotor.setSpeed(0);
    leftBackMotor.setSpeed(0);
  }
  
  Serial.flush();
  delay(10); // Small delay to prevent overwhelming the serial output
}

//------------- Core1 -------------//
void setup1() {
  // configure pio-usb: defined in usbh_helper.h
  rp2040_configure_pio_usb();

  // run host stack on controller (rhport) 1
  USBHost.begin(1);
}

void loop1() {
  USBHost.task();
}

extern "C" {
  // Invoked when device with hid interface is mounted
  void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
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
  void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
    Serial.printf("HIDreport : ");
    for (uint16_t i = 0; i < len; i++) {
      Serial.printf("0x%02X ", report[i]);
    }
    Serial.println();

    if (len >= 4) {  // Ensure we have enough data
      mouse_click = report[0];  // Click
      mouse_x = (int8_t)report[1];  // X movement (cast to signed 8-bit)
      mouse_y = (int8_t)report[2];  // Y movement (cast to signed 8-bit)
      mouse_scroll = (int8_t)report[3];  // Scroll (cast to signed 8-bit)
    }

    // continue to request to receive report
    if (!tuh_hid_receive_report(dev_addr, instance)) {
      Serial.printf("Error: cannot request to receive report\r\n");
    }
  }
}