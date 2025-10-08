/*
 * Robot Car Controller
 * 
 * This code receives commands via Bluetooth (HC-05) from the gesture
 * controller and drives the robot car motors accordingly using L298N
 * motor driver.
 * 
 * Hardware:
 * - Arduino Nano/Uno
 * - HC-05 Bluetooth module (Slave mode)
 * - L298N motor driver module
 * - 2x DC motors
 * 
 * Connections:
 * HC-05: VCC->5V, GND->GND, TX->D0(RX), RX->D1(TX)
 * L298N: IN1->D2, IN2->D4, IN3->D7, IN4->D8
 * Motors: Connect to L298N OUT1-OUT4 terminals
 * 
 * Motor Driver Pins:
 * IN1, IN2 = Control Motor A (Left motor)
 * IN3, IN4 = Control Motor B (Right motor)
 * 
 * Motor Control Logic:
 * HIGH, LOW  = Forward rotation
 * LOW, HIGH  = Reverse rotation
 * LOW, LOW   = Stop
 * HIGH, HIGH = Brake (not used in this code)
 * 
 * Author: [Your Name]
 * Date: October 2025
 * Version: 1.0
 */

// ============================================
// PIN DEFINITIONS
// ============================================
// Define Arduino pins connected to L298N motor driver

// Motor A (Left Motor) control pins
int IN1 = 2;  // Motor A direction control 1
int IN2 = 4;  // Motor A direction control 2

// Motor B (Right Motor) control pins
int IN3 = 7;  // Motor B direction control 1
int IN4 = 8;  // Motor B direction control 2

// Variable to store received command from Bluetooth
char rec;

void setup() {
  // Initialize serial communication at 9600 baud
  // Must match HC-05 Bluetooth module configuration
  Serial.begin(9600);
  
  // Print startup message to Serial Monitor
  Serial.println("AT MODE: READY");
  
  // ============================================
  // CONFIGURE MOTOR CONTROL PINS AS OUTPUTS
  // ============================================
  pinMode(IN1, OUTPUT);  // Motor A control pin 1
  pinMode(IN2, OUTPUT);  // Motor A control pin 2
  pinMode(IN3, OUTPUT);  // Motor B control pin 1
  pinMode(IN4, OUTPUT);  // Motor B control pin 2
  
  // At startup, all pins are LOW (motors stopped)
}

void loop() {
  // ============================================
  // CHECK FOR INCOMING BLUETOOTH DATA
  // ============================================
  // Serial.available() returns the number of bytes available to read
  if (Serial.available() > 0) {
    
    // Read one byte (character) from the serial buffer
    rec = Serial.read();
    
    // ============================================
    // FORWARD COMMAND: 'F'
    // ============================================
    // Both motors rotate forward
    if (rec == 'F') {
      digitalWrite(IN1, HIGH);  // Motor A forward
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);  // Motor B forward
      digitalWrite(IN4, LOW);
    }
    
    // ============================================
    // BACKWARD COMMAND: 'B'
    // ============================================
    // Both motors rotate in reverse
    else if (rec == 'B') {
      digitalWrite(IN1, LOW);   // Motor A reverse
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);   // Motor B reverse
      digitalWrite(IN4, HIGH);
    }
    
    // ============================================
    // LEFT TURN COMMAND: 'L'
    // ============================================
    // Motor A (left) forward, Motor B (right) reverse
    // This creates a pivot turn to the left
    else if (rec == 'L') {
      digitalWrite(IN1, HIGH);  // Motor A forward
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);   // Motor B reverse
      digitalWrite(IN4, HIGH);
    }
    
    // ============================================
    // RIGHT TURN COMMAND: 'R'
    // ============================================
    // Motor A (left) reverse, Motor B (right) forward
    // This creates a pivot turn to the right
    else if (rec == 'R') {
      digitalWrite(IN1, LOW);   // Motor A reverse
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);  // Motor B forward
      digitalWrite(IN4, LOW);
    }
    
    // ============================================
    // STOP COMMAND: 'S'
    // ============================================
    // All motor control pins set to LOW
    // Both motors stop
    else if (rec == 'S') {
      digitalWrite(IN1, LOW);   // Motor A stop
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);   // Motor B stop
      digitalWrite(IN4, LOW);
    }
    
    // If an unrecognized command is received, it's ignored
    // Motors maintain their current state
  }
  
  // Loop repeats continuously, checking for new commands
  // Response time is very fast (milliseconds)
}

/*
 * MOTOR DIRECTION TRUTH TABLE:
 * =============================
 * IN1  IN2  | Motor A Action
 * -----|-----|-----------------
 * LOW  LOW  | Stop (coast)
 * HIGH LOW  | Forward
 * LOW  HIGH | Reverse
 * HIGH HIGH | Brake (not used)
 * 
 * Same logic applies to IN3/IN4 for Motor B
 * 
 * 
 * ROBOT MOVEMENT PATTERNS:
 * ========================
 * Command | Motor A | Motor B | Result
 * --------|---------|---------|-------------
 *    F    | Forward | Forward | Move forward
 *    B    | Reverse | Reverse | Move backward
 *    L    | Forward | Reverse | Turn left (pivot)
 *    R    | Reverse | Forward | Turn right (pivot)
 *    S    | Stop    | Stop    | Stop
 * 
 * 
 * TROUBLESHOOTING:
 * ================
 * 1. Robot moves in wrong direction:
 *    - Swap motor wires at L298N terminals, OR
 *    - Swap HIGH/LOW in the code for that command
 * 
 * 2. Only one motor works:
 *    - Check all wire connections
 *    - Test motor directly with battery
 *    - Verify L298N enable jumpers are ON
 * 
 * 3. Motors weak or slow:
 *    - Check battery voltage (should be >6V)
 *    - Ensure proper power supply to L298N
 *    - Remove 5V jumper on L298N if using >7V battery
 * 
 * 4. No Bluetooth communication:
 *    - Check HC-05 TX/RX not swapped
 *    - Verify baud rate (9600 for car, 115200 for controller)
 *    - Ensure modules are paired correctly
 * 
 * 
 * ENHANCEMENTS YOU CAN ADD:
 * =========================
 * 1. Speed Control (PWM):
 *    - Connect ENA to D3 (PWM), ENB to D5 (PWM)
 *    - Use analogWrite(ENA, speed) where speed = 0-255
 *    - Remove ENA/ENB jumpers on L298N
 * 
 * 2. Smooth Turning:
 *    - For 'L': Run left motor slower instead of reverse
 *    - For 'R': Run right motor slower instead of reverse
 * 
 * 3. Status Feedback:
 *    - Add LED to show current command
 *    - Add buzzer for audio feedback
 *    - Send acknowledgment back to controller
 * 
 * 4. Safety Features:
 *    - Add timeout: stop if no command received for 2 seconds
 *    - Add emergency stop button
 *    - Implement battery voltage monitoring
 * 
 * 
 * EXAMPLE PWM SPEED CONTROL:
 * ==========================
 * #define ENA 3  // Enable pin for Motor A (PWM)
 * #define ENB 5  // Enable pin for Motor B (PWM)
 * 
 * void setup() {
 *   pinMode(ENA, OUTPUT);
 *   pinMode(ENB, OUTPUT);
 *   // ... other setup code ...
 * }
 * 
 * // In forward command:
 * analogWrite(ENA, 200);  // 0-255, adjust for desired speed
 * analogWrite(ENB, 200);
 * digitalWrite(IN1, HIGH);
 * // ... rest of code ...
 * 
 * 
 * PIN CHANGE REFERENCE:
 * =====================
 * If you need to use different pins, update these lines:
 * - Change pin numbers in the definitions at the top
 * - Update pinMode() calls in setup()
 * - Update wiring documentation to match
 * - Ensure new pins can handle OUTPUT mode
 * - For speed control, use PWM-capable pins (3, 5, 6, 9, 10, 11)
 */
