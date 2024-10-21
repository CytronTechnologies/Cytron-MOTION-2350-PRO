#include "usbh_helper.h"
#include "CytronMotorDriver.h"

// Motor configuration
CytronMD rightFrontMotor(PWM_PWM, 8, 9);   // Right front motor
CytronMD rightBackMotor(PWM_PWM, 10, 11);  // Right back motor
CytronMD leftFrontMotor(PWM_PWM, 12, 13);  // Left front motor
CytronMD leftBackMotor(PWM_PWM, 14, 15);   // Left back motor

// Joystick input values
uint16_t input_1 = 0;
uint16_t x_axis = 0;
uint16_t y_axis = 0;

bool system_ready = false;
unsigned long startup_time;
const unsigned long STARTUP_DELAY = 3000; // 3 seconds delay

// Variables for speed multiplier
float speed_multiplier = 1.0;
const float MULTIPLIER_INCREMENT = 0.5;
const float MAX_MULTIPLIER = 10.0;
const float MIN_MULTIPLIER = 0.5;
uint8_t previous_input_1 = 0;

// Function to map joystick values to motor speed
int mapJoystickToSpeed(uint16_t value) {
  return map(value, 0, 255, -255, 255);
}

// Function to apply dead zone
int applyDeadZone(int value, int deadZone) {
  if (abs(value) < deadZone) {
    return 0;
  }
  return value;
}

//------------- Core0 -------------//
void setup() {
  Serial.begin(115200);
  Serial.println("Omnidirectional Robot Control System Starting...");
  
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
    // Handle speed multiplier (assuming button presses are on input_1)
    if ((input_1 & 0x40) && !(previous_input_1 & 0x40)) {  // Assuming 0x01 is for increasing speed
      speed_multiplier = min(speed_multiplier + MULTIPLIER_INCREMENT, MAX_MULTIPLIER);
    } else if ((input_1 & 0x80) && !(previous_input_1 & 0x80)) {  // Assuming 0x02 is for decreasing speed
      speed_multiplier = max(speed_multiplier - MULTIPLIER_INCREMENT, MIN_MULTIPLIER);
    }
    previous_input_1 = input_1;

    // Map joystick movement to motor speeds
    int forward_speed = -mapJoystickToSpeed(y_axis);  // Invert for intuitive control
    int strafe_speed = mapJoystickToSpeed(x_axis);

    // Apply speed multiplier
    forward_speed *= speed_multiplier;
    strafe_speed *= speed_multiplier;

    // Apply dead zone
    int deadZone = 10 * speed_multiplier;  // Adjust as needed
    forward_speed = applyDeadZone(forward_speed, deadZone);
    strafe_speed = applyDeadZone(strafe_speed, deadZone);

    // Calculate individual motor speeds for omnidirectional movement
    int rf_speed = forward_speed - strafe_speed;
    int rb_speed = forward_speed + strafe_speed;
    int lf_speed = forward_speed + strafe_speed;
    int lb_speed = forward_speed - strafe_speed;

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
    Serial.print("Joystick - X: ");
    Serial.print(x_axis);
    Serial.print(", Y: ");
    Serial.print(y_axis);
    Serial.print(", Input1: ");
    Serial.print(input_1, BIN);
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

    if (len >= 7) {  // Ensure we have enough data
      input_1 = report[0];
      x_axis = report[3];
      y_axis = report[4];
    }

    // continue to request to receive report
    if (!tuh_hid_receive_report(dev_addr, instance)) {
      Serial.printf("Error: cannot request to receive report\r\n");
    }
  }
}