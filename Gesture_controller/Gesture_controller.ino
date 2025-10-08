/*
 * Gesture Controller for Robot Car
 * 
 * This code reads accelerometer data from an MPU6050 sensor and sends
 * commands to a robot car via Bluetooth (HC-05) based on hand gestures.
 * 
 * Hardware:
 * - Arduino Nano/Uno
 * - MPU6050 (GY-521) accelerometer/gyroscope module
 * - HC-05 Bluetooth module (Master mode)
 * 
 * Connections:
 * MPU6050: VCC->5V, GND->GND, SDA->A4, SCL->A5
 * HC-05: VCC->5V, GND->GND, TX->D0(RX), RX->D1(TX)
 * 
 * Author: [Your Name]
 * Date: October 2025
 * Version: 1.0
 */

// Include required libraries for MPU6050 sensor
#include <Adafruit_MPU6050.h>  // MPU6050 sensor library
#include <Adafruit_Sensor.h>   // Unified sensor library (dependency)
#include <Wire.h>               // I2C communication library

// Variables to store acceleration values
int x, y;  // X and Y axis acceleration values (converted to integers)

// Create MPU6050 object
Adafruit_MPU6050 mpu;

void setup(void) {
  // Initialize serial communication at 115200 baud
  // This matches the HC-05 Bluetooth module configuration
  Serial.begin(115200);
  
  // Wait for serial port to connect (needed for some boards)
  while (!Serial)
    delay(10);

  // Print startup message to Serial Monitor
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize the MPU6050 sensor
  if (!mpu.begin()) {
    // If sensor is not found, print error and halt
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);  
      // NOTE: There's a syntax error here - missing closing brace
      // The mpu.setAccelerometerRange line should be outside this loop
    }
  }
  
  // Print success message
  Serial.println("MPU6050 Found!");

  // Configure sensor settings
  
  // Set accelerometer range to ±8G
  // Options: 2G, 4G, 8G, 16G (higher = more range, less precision)
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  
  // Set gyroscope range to ±500 degrees/second
  // Options: 250, 500, 1000, 2000 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  
  // Set digital low-pass filter bandwidth to 21 Hz
  // This reduces noise in the readings
  // Options: 260, 184, 94, 44, 21, 10, 5 Hz (lower = smoother but slower)
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("");
  delay(100);  // Short delay to stabilize sensor
}

void loop() {
  // Create event objects to store sensor data
  sensors_event_t a, g, temp;  // a=acceleration, g=gyroscope, temp=temperature
  
  // Read current sensor values
  mpu.getEvent(&a, &g, &temp);
  
  // Convert acceleration values from float to integer
  // Acceleration is measured in m/s^2
  // At rest: X and Y ≈ 0, Z ≈ 9.8 (gravity)
  x = a.acceleration.x;
  y = a.acceleration.y;

  // ============================================
  // DEBUG CODE (currently commented out)
  // Uncomment these lines to see raw sensor values
  // Useful for calibration and troubleshooting
  // ============================================
  
  // Serial.print("Acceleration X: ");
  // Serial.print(a.acceleration.x);
  // Serial.print(", Y: ");
  // Serial.print(a.acceleration.y);
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z);
  // Serial.println(" m/s^2");

  // ============================================
  // GESTURE DETECTION LOGIC
  // ============================================
  // Thresholds determine gesture sensitivity:
  // - Dead zone: -3 to +3 (controller held flat/neutral)
  // - Action threshold: ±4 or more (intentional tilt detected)
  // ============================================

  // STOP: Controller held flat (minimal tilt in any direction)
  if(x >= -3 && x <= 3 && y >= -3 && y <= 3) {
    Serial.println("Stop");   // Print to Serial Monitor
    Serial.write("S");        // Send 'S' command to robot via Bluetooth
  }
  
  // FORWARD: Controller tilted forward (Y axis negative)
  // X axis neutral, Y axis tilted beyond -4
  else if(x >= -3 && x <= 3 && y <= -4) {
    Serial.println("Forward");
    Serial.write("F");        // Send 'F' command to robot
  }
  
  // BACKWARD: Controller tilted backward (Y axis positive)
  // X axis neutral, Y axis tilted beyond +4
  else if(x >= -3 && x <= 3 && y >= 4) {
    Serial.println("Backward");
    Serial.write("B");        // Send 'B' command to robot
  }
  
  // LEFT: Controller tilted right (X axis positive)
  // NOTE: Due to sensor orientation, tilting right makes robot turn left
  // X axis tilted beyond +4, Y axis neutral
  else if(x >= 4 && y >= -3 && y <= 3) {
    Serial.println("Left");
    Serial.write("L");        // Send 'L' command to robot
  }
  
  // RIGHT: Controller tilted left (X axis negative)
  // NOTE: Due to sensor orientation, tilting left makes robot turn right
  // X axis tilted beyond -4, Y axis neutral
  else if(x <= -4 && y >= -3 && y <= 3) {
    Serial.println("Right");
    Serial.write("R");        // Send 'R' command to robot
  }
  
  // Loop repeats continuously, checking for gestures
  // No delay here means very fast response time
  // Consider adding delay(50-100) if readings are too jittery
}

/*
 * CALIBRATION TIPS:
 * ==================
 * 1. Uncomment the debug Serial.print lines
 * 2. Open Serial Monitor at 115200 baud
 * 3. Hold controller flat and note X, Y values
 * 4. Tilt in each direction and note peak values
 * 5. Adjust thresholds (-3, +3, -4, +4) based on your readings
 * 
 * COMMON ISSUES:
 * ==============
 * - Erratic commands: Increase dead zone (e.g., -5 to +5)
 * - Not sensitive enough: Decrease action threshold (e.g., ±3)
 * - Wrong directions: Sensor mounted upside down or rotated
 * 
 * MODIFICATIONS:
 * ==============
 * - Add delay(100) at end of loop for more stable readings
 * - Implement averaging filter for smoother gesture detection
 * - Add diagonal movement (e.g., forward-left, forward-right)
 * - Map tilt angle to motor speed (PWM control)
 */
