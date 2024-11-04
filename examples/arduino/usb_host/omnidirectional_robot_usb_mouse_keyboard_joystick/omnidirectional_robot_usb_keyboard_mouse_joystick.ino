#include "usbh_helper.h"
#include "CytronMotorDriver.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Motor configuration
CytronMD rightFrontMotor(PWM_PWM, 8, 9);   // Right front motor
CytronMD rightBackMotor(PWM_PWM, 10, 11);  // Right back motor
CytronMD leftFrontMotor(PWM_PWM, 12, 13);  // Left front motor
CytronMD leftBackMotor(PWM_PWM, 14, 15);   // Left back motor

// Mode selection pins
const int BTN_20 = 20;
const int BTN_21 = 21;

// I2C pins for OLED
const int SDA_PIN = 16;
const int SCL_PIN = 17;

// Battery voltage
int battPin = 29;    
int raw_value = 0; 
float voltage_value = 0; 
float vin_voltage_value = 0; // solder the jumper at back to measure VIN
float batt_value = 0;

// Control mode
enum ControlMode {
  MOUSE,
  JOYSTICK,
  KEYBOARD
};

ControlMode currentMode = MOUSE;
const int NUM_MODES = 3;

// Joystick input values
uint16_t input_1 = 0;
uint16_t x_axis = 0;
uint16_t y_axis = 0;

// Mouse input values
int8_t mouse_x = 0;
int8_t mouse_y = 0;
uint8_t mouse_buttons = 0;
int8_t mouse_scroll = 0;

// Keyboard input values
uint8_t key_shift = 0;
uint8_t key1 = 0;
uint8_t key2 = 0;


bool system_ready = false;
unsigned long startup_time;
const unsigned long STARTUP_DELAY = 3000;  // 3 seconds delay

// Variables for speed multiplier
float speed_multiplier = 1.0;
const float MULTIPLIER_INCREMENT = 0.5;
const float MAX_MULTIPLIER = 10.0;
const float MIN_MULTIPLIER = 0.5;
uint8_t previous_input_1 = 0;
uint8_t previous_mouse_buttons = 0;
uint8_t previous_key = 0;
int data_len = 0;

// Variables for scroll handling
int accumulated_scroll = 0;
const int SCROLL_THRESHOLD = 1;
const int MAX_ROTATION_SPEED = 128;  // Reduced from 128 for finer control

// Nerf the speed
const float joy_nerf = 0.3;
const float mouse_nerf = 1;
const float key_nerf = 0.3;

// EEPROM address for storing the mode
#define MODE_EEPROM_ADDR 0

// Function to save mode to EEPROM
void saveModeToEEPROM(ControlMode mode) {
  EEPROM.begin(512);  // Initialize EEPROM with 512 bytes
  EEPROM.write(MODE_EEPROM_ADDR, static_cast<uint8_t>(mode));
  EEPROM.commit();
  EEPROM.end();
}

// Function to map joystick/mouse values to motor speed
int mapInputToSpeed(int value, int max_input) {
  return map(value, -max_input, max_input, -255, 255);
}

// Function to apply dead zone
int applyDeadZone(int value, int deadZone) {
  if (abs(value) < deadZone) {
    return 0;
  }
  return value;
}

// Function to reset USB connection
void resetUSBConnection() {
  Serial.println("Resetting USB connection...");
  USBHost.task();
  delay(100);
  rp2040_configure_pio_usb();
  USBHost.begin(1);
  delay(1000);
  Serial.println("USB connection reset complete.");
}

// Function to update OLED display
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (currentMode == MOUSE) {
    display.println("Mouse");
  } else if (currentMode == JOYSTICK) {
    display.println("Joystick");
  } else if (currentMode == KEYBOARD) {
    display.println("Keyboard");
  }
  display.setTextSize(1);
  display.print("Multiplier: ");
  display.println(speed_multiplier);

  // Add mode change instructions
  display.setTextSize(1);
  display.println();
  display.println();
  display.println("Change Mode:");
  display.println();
  display.print("<- (BTN20) ");
  display.println("(BTN21) ->");

  display.display();
}

// Function to handle scroll input
int handleScrollInput(int8_t scroll_input) {
  accumulated_scroll += scroll_input;

  if (abs(accumulated_scroll) >= SCROLL_THRESHOLD) {
    int rotation = (accumulated_scroll > 0) ? MAX_ROTATION_SPEED : -MAX_ROTATION_SPEED;
    accumulated_scroll = 0;  // Reset accumulated scroll
    return (int)rotation;
  }

  return 0;  // No rotation if threshold not met
}

// Function to read battery percentage
float readBattery() {
  // read the value from the sensor:
  raw_value = analogRead(battPin);
  voltage_value = (raw_value * 3.3) / 4095;
  vin_voltage_value = voltage_value * 6.1;
  return vin_voltage_value;
}

// Function to load mode from EEPROM
ControlMode loadModeFromEEPROM() {
  EEPROM.begin(512);  // Initialize EEPROM with 512 bytes
  uint8_t savedMode = EEPROM.read(MODE_EEPROM_ADDR);
  EEPROM.end();

  if (savedMode >= NUM_MODES) {
    return MOUSE;  // Default to MOUSE if saved mode is invalid
  }
  return static_cast<ControlMode>(savedMode);
}

// Function to cycle through modes
void cycleMode(bool forward) {
  int modeIndex = static_cast<int>(currentMode);
  if (forward) {
    modeIndex = (modeIndex + 1) % NUM_MODES;
  } else {
    modeIndex = (modeIndex - 1 + NUM_MODES) % NUM_MODES;
  }
  currentMode = static_cast<ControlMode>(modeIndex);

  Serial.print("Switching to ");
  Serial.print(currentMode == MOUSE ? "Mouse" : (currentMode == JOYSTICK ? "Joystick" : "Keyboard"));
  Serial.println(" mode");

  saveModeToEEPROM(currentMode);  // Save the new mode to EEPROM
  resetUSBConnection();
  updateDisplay();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Omnidirectional Robot Control System Starting...");

  // Load the saved mode from EEPROM
  currentMode = loadModeFromEEPROM();
  Serial.print("Loaded mode from EEPROM: ");
  Serial.println(currentMode == MOUSE ? "Mouse" : (currentMode == JOYSTICK ? "Joystick" : "Keyboard"));

  // Initialize OLED display
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Initialize motors to stopped state
  rightFrontMotor.setSpeed(0);
  rightBackMotor.setSpeed(0);
  leftFrontMotor.setSpeed(0);
  leftBackMotor.setSpeed(0);

  // Set up mode selection pins
  pinMode(BTN_20, INPUT_PULLUP);
  pinMode(BTN_21, INPUT_PULLUP);

  // Set up battery pin
  pinMode(battPin, INPUT);
  // enable adc resolution to 12-bit (default 10-bit)
  analogReadResolution(12);

  startup_time = millis();

  updateDisplay();  // Initial display update
}

void loop() {
  // Check if startup delay has passed
  if (!system_ready && (millis() - startup_time >= STARTUP_DELAY)) {
    system_ready = true;
    Serial.println("System Ready!");
  }

  if (system_ready) {
    // Check for mode changes
    if (digitalRead(BTN_20) == LOW) {
      cycleMode(false);  // Cycle backwards
      delay(200);        // Debounce delay
    } else if (digitalRead(BTN_21) == LOW) {
      cycleMode(true);  // Cycle forwards
      delay(200);       // Debounce delay
    }

    int forward_speed = 0;
    int strafe_speed = 0;
    int rotation_speed = 0;

    if (currentMode == JOYSTICK && data_len == 9) {  //Joystick
      // Handle speed multiplier for joystick
      if ((input_1 & 0x40) && !(previous_input_1 & 0x40)) {
        speed_multiplier = min(speed_multiplier + MULTIPLIER_INCREMENT, MAX_MULTIPLIER);
        updateDisplay();
      } else if ((input_1 & 0x80) && !(previous_input_1 & 0x80)) {
        speed_multiplier = max(speed_multiplier - MULTIPLIER_INCREMENT, MIN_MULTIPLIER);
        updateDisplay();
      }
      previous_input_1 = input_1;

      // Map joystick movement to motor speeds
      forward_speed = -mapInputToSpeed(y_axis - 128, 128) * joy_nerf;  // Center at 128
      strafe_speed = mapInputToSpeed(x_axis - 128, 128) * joy_nerf;    // Center at 128
    } else if (currentMode == MOUSE && data_len == 8) {                // Mouse
      // Handle speed multiplier for mouse
      if ((mouse_buttons & 0x01) && !(previous_mouse_buttons & 0x01)) {
        speed_multiplier = min(speed_multiplier + MULTIPLIER_INCREMENT, MAX_MULTIPLIER);
        updateDisplay();
      } else if ((mouse_buttons & 0x02) && !(previous_mouse_buttons & 0x02)) {
        speed_multiplier = max(speed_multiplier - MULTIPLIER_INCREMENT, MIN_MULTIPLIER);
        updateDisplay();
      }
      previous_mouse_buttons = mouse_buttons;

      // Map mouse movement to motor speeds
      forward_speed = -mapInputToSpeed(mouse_y, 127) * mouse_nerf;
      strafe_speed = mapInputToSpeed(mouse_x, 127) * mouse_nerf;
      rotation_speed = handleScrollInput(mouse_scroll) * mouse_nerf;
    } else if (currentMode == KEYBOARD && data_len == 8) {  // Keyboard
      // Handle speed multiplier for keyboard
      if ((key1 == 0x2E) && !(previous_key == 0x2E)) {
        speed_multiplier = min(speed_multiplier + MULTIPLIER_INCREMENT, MAX_MULTIPLIER);
        updateDisplay();
      } else if ((key1 == 0x2D) && !(previous_key == 0x2D)) {
        speed_multiplier = max(speed_multiplier - MULTIPLIER_INCREMENT, MIN_MULTIPLIER);
        updateDisplay();
      }
      previous_key = key1;

      if (key1 == 0x52 && key2 == 0x4F || key1 == 0x4F && key2 == 0x52) {
        forward_speed = 64;  //forward & right
        strafe_speed = 64;
      } else if (key1 == 0x52 && key2 == 0x50 || key1 == 0x50 && key2 == 0x52) {
        forward_speed = 64;  //forward & left
        strafe_speed = -64;
      } else if (key1 == 0x51 && key2 == 0x4F || key1 == 0x4F && key2 == 0x51) {
        forward_speed = -64;  //backward & right
        strafe_speed = 64;
      } else if (key1 == 0x51 && key2 == 0x50 || key1 == 0x50 && key2 == 0x51) {
        forward_speed = -64;  //backward & left
        strafe_speed = -64;
      } else if (key1 == 0x50 && key_shift == 0x20) {
        rotation_speed = MAX_ROTATION_SPEED;  //rotate right
      } else if (key1 == 0x4F && key_shift == 0x20) {
        rotation_speed = -MAX_ROTATION_SPEED; //rotate left
      } else if (key1 == 0x52) {  //forward
        forward_speed = 127;  
      } else if (key1 == 0x51) {  //backward
        forward_speed = -127;  
      } else if (key1 == 0x50) {  // to left
        strafe_speed = -127;
      } else if (key1 == 0x4F) {  // to right
        strafe_speed = 127;
      }

      forward_speed *= key_nerf;
      strafe_speed *= key_nerf;
      rotation_speed *= key_nerf;
    }

    // Apply speed multiplier
    forward_speed *= speed_multiplier;
    strafe_speed *= speed_multiplier;
    rotation_speed *= speed_multiplier;

    // Apply dead zone
    int deadZone = 10 * speed_multiplier;  // Adjust as needed
    forward_speed = applyDeadZone(forward_speed, deadZone);
    strafe_speed = applyDeadZone(strafe_speed, deadZone);

    // Calculate individual motor speeds for omnidirectional movement
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
    Serial.print("Mode: ");
    Serial.print(currentMode == MOUSE ? "Mouse" : (currentMode == JOYSTICK ? "Joystick" : "Keyboard"));
    Serial.print(" | X: ");
    Serial.print(currentMode == MOUSE ? mouse_x : x_axis);
    Serial.print(", Y: ");
    Serial.print(currentMode == MOUSE ? mouse_y : y_axis);
    Serial.print(", Buttons: ");
    Serial.print(currentMode == MOUSE ? mouse_buttons : input_1, BIN);
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
  } else {
    // System not ready, keep motors stopped
    rightFrontMotor.setSpeed(0);
    rightBackMotor.setSpeed(0);
    leftFrontMotor.setSpeed(0);
    leftBackMotor.setSpeed(0);
  }

  // Reset mouse movement and scroll to stop when no new input
  mouse_x = 0;
  mouse_y = 0;
  mouse_scroll = 0;

  Serial.flush();
  delay(10);  // Small delay to prevent overwhelming the serial output
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
    data_len = len;
    if (currentMode == JOYSTICK && data_len == 9) {
      input_1 = report[0];
      x_axis = report[3];
      y_axis = report[4];
    } else if (currentMode == MOUSE && data_len == 8) {
      mouse_buttons = report[1];
      mouse_x = (int8_t)report[2];
      mouse_y = (int8_t)report[4];
      mouse_scroll = (int8_t)report[6];
    } else if (currentMode == KEYBOARD && data_len == 8) {
      key_shift = report[0];
      key1 = report[2];
      key2 = report[3];
    }

    // continue to request to receive report
    if (!tuh_hid_receive_report(dev_addr, instance)) {
      Serial.printf("Error: cannot request to receive report\r\n");
    }
  }
}
