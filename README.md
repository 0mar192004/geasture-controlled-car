# Gesture-Controlled Robot Car

A gesture-controlled robot car using MPU6050 accelerometer and HC-05 Bluetooth modules for wireless communication.

## Overview

This project consists of two Arduino systems:
1. **Gesture Controller** - Uses MPU6050 to detect hand tilts and send commands
2. **Robot Car** - Receives commands via Bluetooth and controls motors accordingly

## Hardware Requirements

### Gesture Controller
- Arduino nano board
- MPU6050 accelerometer/gyroscope module
- HC-05 Bluetooth module (Master)
- Power supply

### Robot Car
- Arduino uno board
- HC-05 Bluetooth module (Slave)
- L298N motor driver or similar
- 2x DC motors
- Robot car chassis
- Power supply

## Pin Configuration

### Robot Car (Robot_car.ino)
- `IN1` → Pin 2 (Motor A control)
- `IN2` → Pin 4 (Motor A control)
- `IN3` → Pin 7 (Motor B control)
- `IN4` → Pin 8 (Motor B control)

## HC-05 Bluetooth Configuration

### Step 1: Configure Slave (Robot Car)

1. Enter AT command mode on the HC-05
2. Set UART baud rate:
   ```
   AT+UART=115200,0,0
   ```
3. Set device as slave:
   ```
   AT+ROLE=0
   ```
4. Get the slave address (note this down):
   ```
   AT+ADDR?
   ```
   Example response: `0022:06:01543B`

### Step 2: Configure Master (Gesture Controller)

1. Enter AT command mode on the HC-05
2. Set UART baud rate:
   ```
   AT+UART=115200,0,0
   ```
3. Set device as master:
   ```
   AT+ROLE=1
   ```
4. Set connection mode to fixed address:
   ```
   AT+CMODE=0
   ```
5. Bind to slave address (use address from Step 1):
   ```
   AT+BIND=0022,06,01543B
   ```
   **Note:** Replace colons with commas in the address

## Gesture Controls

| Gesture | X Range | Y Range | Action | Command |
|---------|---------|---------|--------|---------|
| **Neutral** | -3 to 3 | -3 to 3 | Stop | S |
| **Tilt Forward** | -3 to 3 | ≤ -4 | Move Forward | F |
| **Tilt Backward** | -3 to 3 | ≥ 4 | Move Backward | B |
| **Tilt Right** | ≥ 4 | -3 to 3 | Turn Left* | L |
| **Tilt Left** | ≤ -4 | -3 to 3 | Turn Right* | R |

*The gesture direction is inverted relative to the robot's movement due to sensor orientation.

## Installation

### Libraries Required
Install the following libraries via Arduino IDE Library Manager:
- Adafruit MPU6050
- Adafruit Unified Sensor

### Upload Instructions

1. Upload `Robot_car.ino` to the robot car Arduino
2. Upload `Gesture_controller.ino` to the gesture controller Arduino
3. Configure both HC-05 modules as described above
4. Power on both systems - they should connect automatically

## Usage

1. Power on the robot car (slave device)
2. Power on the gesture controller (master device)
3. Wait for Bluetooth connection (HC-05 LED will stop blinking rapidly)
4. Hold the gesture controller flat to stop
5. Tilt in different directions to control the robot:
   - Forward tilt → Robot moves forward
   - Backward tilt → Robot moves backward
   - Left tilt → Robot turns right
   - Right tilt → Robot turns left

## Troubleshooting

- **Devices won't pair**: Double-check the slave address and ensure it's correctly bound on the master
- **No response from robot**: Verify baud rates match on both systems (115200 for gesture controller, 9600 for robot car serial)
- **Erratic movements**: Calibrate the MPU6050 by keeping it flat during startup
- **HC-05 won't respond to AT commands**: Ensure you're in AT mode (hold button during power-on for most modules)

## Serial Monitor Output

### Gesture Controller (115200 baud)
Displays current gesture: "Stop", "Forward", "Backward", "Left", or "Right"

### Robot Car (9600 baud)
Displays "AT MODE: READY" on startup

## License

Open source - feel free to modify and improve!

## Contributors

-Moaz Elkholy
-Aliaa Abdelnaby
-Zead Reda
-Salma Khaled
-Omar khaled
-Jana Khaled
---

**Note:** Ensure proper power supply for motors. The motor driver should be powered separately from the Arduino to avoid voltage drops.
