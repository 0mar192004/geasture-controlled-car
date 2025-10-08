# Troubleshooting Guide

This guide covers common issues and solutions for the gesture-controlled robot car project.

---

## Table of Contents
1. [Bluetooth Connection Issues](#bluetooth-connection-issues)
2. [Motor Problems](#motor-problems)
3. [Gesture Detection Issues](#gesture-detection-issues)
4. [Power Issues](#power-issues)
5. [Code Upload Problems](#code-upload-problems)
6. [Performance Issues](#performance-issues)

---

## Bluetooth Connection Issues

### HC-05 Won't Enter AT Mode

**Symptoms:**
- No response to AT commands
- Can't configure module

**Solutions:**
1. **Hold button during power-on**
   - Press and hold the button on HC-05
   - Connect power while holding
   - LED should blink slowly (about 2 seconds interval)
   - Release button after LED starts blinking

2. **Check baud rate**
   - Default AT mode baud rate is often 38400
   - Try: `Serial.begin(38400);`
   - Some modules use 9600 in AT mode

3. **Use correct pins**
   - Connect HC-05 TX to Arduino RX
   - Connect HC-05 RX to Arduino TX
   - May need voltage divider for RX (5V → 3.3V)

### HC-05 Modules Won't Pair

**Symptoms:**
- Both modules power on but don't connect
- LEDs keep blinking rapidly on both

**Solutions:**
1. **Verify ROLE settings**
   ```
   Master: AT+ROLE=1
   Slave:  AT+ROLE=0
   ```

2. **Check address binding**
   - Get slave address: `AT+ADDR?`
   - Bind on master: `AT+BIND=XXXX,XX,XXXXXX`
   - Ensure commas (not colons) in bind command

3. **Reset to factory settings**
   ```
   AT+ORGL
   AT+RESET
   ```

4. **Check CMODE**
   - Master should be: `AT+CMODE=0` (fixed address)

5. **Power cycle both modules**
   - Turn off both completely
   - Turn on slave first
   - Then turn on master
   - Wait 10-15 seconds for pairing

### Modules Pair But No Data Transfer

**Symptoms:**
- HC-05 LEDs show paired (slow blink or solid)
- No commands received by robot

**Solutions:**
1. **Check baud rates match**
   - Gesture controller: 115200
   - Robot car: 9600
   - HC-05 modules: Should both be 115200
   - Command: `AT+UART=115200,0,0`

2. **Verify TX/RX not swapped**
   - Arduino TX → HC-05 RX
   - Arduino RX → HC-05 TX

3. **Disconnect during programming**
   - Remove HC-05 TX/RX pins when uploading code
   - Serial pins conflict with USB programming

4. **Check for voltage issues**
   - Ensure HC-05 receives stable 3.3-5V
   - Use capacitor (100µF) near VCC if voltage is unstable

### Connection Drops Randomly

**Symptoms:**
- Works initially but disconnects
- Intermittent response

**Solutions:**
1. **Reduce distance** - Start close (1-2 meters) and test
2. **Check power supply** - Low battery causes connection issues
3. **Minimize interference**
   - Keep away from WiFi routers
   - Move away from metal objects
   - Avoid motor noise (add capacitors to motors)
4. **Add decoupling capacitors** - 100µF near HC-05 VCC/GND

---

## Motor Problems

### Motors Don't Spin

**Symptoms:**
- Code uploads successfully
- No motor movement

**Solutions:**
1. **Check power supply**
   - Motors need separate power from logic
   - Verify battery is charged (>6V)
   - Test battery voltage with multimeter

2. **Test motors directly**
   - Disconnect from motor driver
   - Connect motor wires directly to battery
   - If motor spins → driver issue
   - If motor doesn't spin → motor issue

3. **Verify motor driver connections**
   ```
   Arduino Pin → L298N
   2 → IN1
   4 → IN2
   7 → IN3
   8 → IN4
   GND → GND
   ```

4. **Check L298N jumpers**
   - Remove 5V jumper if using >7V battery
   - Enable jumpers should be ON (EN A, EN B)

5. **Test with simple code**
   ```cpp
   void setup() {
     pinMode(2, OUTPUT);
     pinMode(4, OUTPUT);
     digitalWrite(2, HIGH);
     digitalWrite(4, LOW);
   }
   void loop() {}
   ```

### Motors Spin in Wrong Direction

**Symptoms:**
- Forward command makes robot go backward
- Left/Right reversed

**Solutions:**
1. **Swap motor wires** at motor driver terminals
2. **Or modify code:**
   ```cpp
   // Swap HIGH/LOW in the code
   if (rec == 'F') {
     digitalWrite(IN1, LOW);  // Changed
     digitalWrite(IN2, HIGH); // Changed
     digitalWrite(IN3, LOW);  // Changed
     digitalWrite(IN4, HIGH); // Changed
   }
   ```

### Motors Spin Weakly

**Symptoms:**
- Motors turn but with low power
- Robot struggles to move

**Solutions:**
1. **Check battery voltage**
   - Should be >6V for most motors
   - Charge or replace batteries

2. **Reduce load**
   - Remove extra weight
   - Check wheels aren't rubbing

3. **Use PWM for speed control**
   ```cpp
   // Add speed control
   analogWrite(ENA, 200); // 0-255
   analogWrite(ENB, 200);
   ```

4. **Check motor driver heat**
   - If driver is very hot, it may be current limiting
   - Use heat sink on L298N
   - Switch to higher current driver

### One Motor Works, One Doesn't

**Symptoms:**
- One motor spins, other is dead
- Robot moves in circles

**Solutions:**
1. **Check all connections** for non-working motor
2. **Test motor directly** with battery
3. **Swap motors** to isolate issue (motor vs driver)
4. **Check driver channel**
   - L298N may have one bad channel
   - Try connecting motor to working channel
   - Replace driver if necessary

---

## Gesture Detection Issues

### MPU6050 Not Detected

**Symptoms:**
- Serial output: "Failed to find MPU6050 chip"
- Code hangs at startup

**Solutions:**
1. **Check I2C connections**
   ```
   MPU6050 → Arduino Uno
   SDA → A4
   SCL → A5
   VCC → 5V
   GND → GND
   ```
   For other boards, use dedicated SDA/SCL pins

2. **Run I2C scanner**
   ```cpp
   // Search "I2C scanner" in Arduino examples
   // Should detect device at 0x68 or 0x69
   ```

3. **Check address jumper**
   - Default address: 0x68
   - If AD0 pin is HIGH: 0x69
   - Modify code if needed:
   ```cpp
   mpu.begin(0x69); // If using alternate address
   ```

4. **Try different power source**
   - MPU6050 can be sensitive to power quality
   - Add 100µF capacitor near VCC/GND

5. **Check for damaged sensor**
   - Try another MPU6050 module
   - Inspect for physical damage

### Gestures Not Detected or Incorrect

**Symptoms:**
- Tilting controller doesn't register
- Wrong commands sent
- Always shows "Stop"

**Solutions:**
1. **Check Serial Monitor output**
   - Uncomment debug lines in code:
   ```cpp
   Serial.print("X: ");
   Serial.print(a.acceleration.x);
   Serial.print(", Y: ");
   Serial.println(a.acceleration.y);
   ```

2. **Calibrate thresholds**
   - Current thresholds: ±3 for neutral, ±4 for action
   - Adjust based on your sensor readings:
   ```cpp
   // Increase sensitivity (trigger easier)
   if(x >= -2 && x<= 2 && y <= -3)
   
   // Decrease sensitivity (require more tilt)
   if(x >= -4 && x<= 4 && y <= -5)
   ```

3. **Check sensor orientation**
   - MPU6050 chip should face up
   - Ensure you're tilting on correct axis
   - Mark "forward" direction clearly

4. **Add delay for stability**
   ```cpp
   void loop() {
     // ... existing code ...
     delay(100); // Add at end of loop
   }
   ```

### Erratic/Jittery Readings

**Symptoms:**
- Commands change rapidly
- Robot twitches or stops frequently
- Unstable gesture detection

**Solutions:**
1. **Increase dead zone**
   ```cpp
   // Current: ±3
   // Try: ±4 or ±5
   if(x >= -5 && x<= 5 && y >= -5 && y<= 5)
   ```

2. **Add averaging filter**
   ```cpp
   float xSum = 0, ySum = 0;
   for(int i=0; i<5; i++) {
     sensors_event_t a, g, temp;
     mpu.getEvent(&a, &g, &temp);
     xSum += a.acceleration.x;
     ySum += a.acceleration.y;
     delay(10);
   }
   x = xSum / 5;
   y = ySum / 5;
   ```

3. **Use built-in filter**
   - Already in code: `mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);`
   - Try lower bandwidth: `MPU6050_BAND_10_HZ` or `MPU6050_BAND_5_HZ`

4. **Check for vibration**
   - Secure MPU6050 firmly
   - Isolate from motor vibrations
   - Use foam padding

---

## Power Issues

### Arduino Resets Randomly

**Symptoms:**
- Arduino restarts unexpectedly
- Serial output shows startup message repeatedly

**Solutions:**
1. **Separate power supplies**
   - Motors should NOT share Arduino's power
   - Use separate battery for motors
   - Connect only GND between systems

2. **Add capacitors**
   - 1000µF capacitor across motor power supply
   - 100µF near Arduino VIN
   - Reduces voltage spikes

3. **Check current draw**
   - Arduino + HC-05 + MPU6050 ≈ 100-150mA
   - Motors can draw 500mA-2A each
   - Ensure power supply can handle total load

### Battery Drains Quickly

**Symptoms:**
- Fresh batteries die in minutes
- Voltage drops rapidly during use

**Solutions:**
1. **Check for shorts**
   - Use multimeter to check resistance
   - Should be high (>100Ω) when off

2. **Use higher capacity batteries**
   - Switch to 18650 Li-ion (2000-3000mAh)
   - Or larger AA batteries (2500mAh+)

3. **Add power management**
   - Use sleep mode when idle
   - Turn off motors when stopped
   - Add auto-shutoff after inactivity

4. **Check motor efficiency**
   - Stalled motors draw excessive current
   - Ensure wheels spin freely
   - Check for mechanical binding

### HC-05 LED Not Lighting

**Symptoms:**
- No LED activity on HC-05
- Module appears dead

**Solutions:**
1. **Check power connections**
   - VCC should be 3.3-5V
   - Measure voltage with multimeter
   - Ensure good contact on pins

2. **Check polarity**
   - Verify VCC and GND not reversed
   - Some modules have reverse polarity protection, some don't

3. **Test with external power**
   - Connect HC-05 directly to 5V power supply
   - If LED works → Arduino power issue
   - If LED doesn't work → module may be damaged

---

## Code Upload Problems

### "avrdude: stk500_recv(): programmer is not responding"

**Symptoms:**
- Code won't upload to Arduino
- Error message about programmer

**Solutions:**
1. **Disconnect HC-05 TX/RX pins**
   - Serial pins (0 and 1) conflict with USB
   - Remove these connections during upload
   - Reconnect after successful upload

2. **Select correct board**
   - Tools → Board → Arduino Nano/Uno
   - Check processor (Nano has old bootloader option)

3. **Select correct COM port**
   - Tools → Port → (Your Arduino port)
   - Windows: COM3, COM4, etc.
   - Mac: /dev/cu.usbserial-*
   - Linux: /dev/ttyUSB0, /dev/ttyACM0

4. **Press reset button**
   - Try uploading while pressing reset
   - Release when "Uploading..." appears

### Compilation Errors

**Symptoms:**
- Code won't compile
- Error messages about missing libraries

**Solutions:**
1. **Install required libraries**
   - See required-libraries.txt
   - Sketch → Include Library → Manage Libraries
   - Search and install: Adafruit MPU6050, Adafruit Unified Sensor

2. **Check library versions**
   - Update to latest versions
   - Library Manager → Update All

3. **Close and reopen Arduino IDE**
   - Refresh library cache
   - Restart can fix recognition issues

### Code Uploads But Doesn't Run

**Symptoms:**
- Upload succeeds
- Serial Monitor shows nothing or garbage

**Solutions:**
1. **Check baud rate**
   - Serial Monitor baud must match code
   - Gesture controller: 115200
   - Robot car: 9600

2. **Reconnect HC-05**
   - Connect TX/RX after upload
   - Ensure correct pins

3. **Add delay in setup()**
   ```cpp
   void setup() {
     Serial.begin(115200);
     delay(2000); // Wait for Serial
     Serial.println("Starting...");
   }
   ```

---

## Performance Issues

### Slow Response Time

**Symptoms:**
- Robot responds slowly to gestures
- 1-2 second delay

**Solutions:**
1. **Reduce serial printing**
   - Comment out Serial.println() in main loop
   - Printing slows execution

2. **Remove delays**
   - Check for unnecessary delay() calls
   - Use millis() for non-blocking delays

3. **Optimize code**
   ```cpp
   // Instead of multiple if-else
   // Use switch-case
   switch(rec) {
     case 'F': /* forward */ break;
     case 'B': /* back */ break;
     // etc
   }
   ```

4. **Check Bluetooth**
   - 115200 baud should be fast
   - Ensure both modules configured correctly

### Limited Range

**Symptoms:**
- Only works within 1-2 meters
- Connection drops at distance

**Solutions:**
1. **Check antenna**
   - Some HC-05 modules have removable antennas
   - Ensure antenna is properly attached
   - Don't touch antenna during operation

2. **Reduce interference**
   - Turn off nearby WiFi devices
   - Move away from walls and metal objects
   - Keep away from other 2.4GHz devices

3. **Add external antenna**
   - Some HC-05 modules support external antennas
   - Can extend range to 30-100 meters

4. **Check for obstructions**
   - Bluetooth is line-of-sight
   - Avoid barriers between devices

### Inconsistent Behavior

**Symptoms:**
- Sometimes works, sometimes doesn't
- Random errors or lockups

**Solutions:**
1. **Check all connections**
   - Loose wires cause intermittent issues
   - Solder connections for reliability
   - Use proper crimps on jumper wires

2. **Add error handling**
   ```cpp
   if (!mpu.begin()) {
     Serial.println("MPU6050 error!");
     // Try to reinitialize
     delay(1000);
   }
   ```

3. **Monitor power supply**
   - Voltage drops cause issues
   - Use multimeter to check voltage during operation
   - Should remain stable under load

4. **Reset Arduino periodically**
   - Add watchdog timer
   - Or manual reset button

---

## Advanced Troubleshooting

### Using Serial Monitor for Debugging

1. **Enable verbose output**
   ```cpp
   Serial.print("X: ");
   Serial.print(x);
   Serial.print(" Y: ");
   Serial.print(y);
   Serial.print(" Cmd: ");
   Serial.println(rec);
   ```

2. **Check received commands**
   ```cpp
   if(Serial.available() > 0) {
     rec = Serial.read();
     Serial.print("Received: ");
     Serial.println(rec);
   }
   ```

3. **Monitor timing**
   ```cpp
   unsigned long t = millis();
   // ... code ...
   Serial.print("Loop time: ");
   Serial.println(millis() - t);
   ```

### Using Multimeter

1. **Check voltages**
   - Battery: Should be >6V
   - Arduino 5V pin: 4.8-5.2V
   - HC-05 VCC: 3.3-5V
   - Motor driver input: Match battery voltage

2. **Check continuity**
   - Set to continuity mode (beep)
   - Test all wire connections
   - Find broken wires or bad solder joints

3. **Measure current draw**
   - Set to DC current mode
   - Place in series with power supply
   - Normal: <500mA idle, <2A running

### Component Testing

1. **Test HC-05 with phone**
   - Pair HC-05 with smartphone
   - Use Bluetooth terminal app
   - Send commands manually
   - If works with phone → Arduino issue
   - If doesn't work → HC-05 issue

2. **Test MPU6050 with example code**
   - File → Examples → Adafruit MPU6050 → basic_readings
   - Should show changing values when moved
   - If works → gesture code issue
   - If doesn't work → sensor issue

3. **Test motors independently**
   - Disconnect from motor driver
   - Connect to battery via switch
   - Should spin freely and strongly

---

## Getting Help

If you're still stuck after trying these solutions:

1. **Document your issue**
   - Take clear photos of wiring
   - Copy full error messages
   - Note what you've already tried

2. **Check resources**
   - Arduino Forum: https://forum.arduino.cc
   - Project GitHub Issues page
   - HC-05 datasheets and guides

3. **Provide details when asking**
   - Arduino board type
   - Library versions
   - Power supply specifications
   - Error messages
   - Photos of setup

4. **Test systematically**
   - Isolate components
   - Test one thing at a time
   - Document what works and what doesn't

---

## Common Error Messages Reference

| Error Message | Likely Cause | Solution |
|--------------|--------------|----------|
| "Failed to find MPU6050" | I2C connection issue | Check SDA/SCL wiring |
| "avrdude: programmer not responding" | Serial port conflict | Disconnect HC-05 TX/RX |
| "Compilation error: missing library" | Library not installed | Install via Library Manager |
| No Serial output | Wrong baud rate | Match Serial.begin() rate |
| Motors hum but don't spin | Insufficient power | Check battery, increase voltage |
| Robot moves erratically | Threshold too sensitive | Increase dead zone values |
| HC-05 won't pair | Wrong configuration | Re-run AT commands |

---

## Prevention Tips

To avoid issues in the first place:

1. **Double-check before powering on**
   - Verify all connections
   - Check polarity
   - Look for shorts

2. **Use quality components**
   - Genuine Arduino recommended
   - Name-brand motors
   - Fresh batteries

3. **Proper wire management**
   - Secure all connections
   - Strain relief on wires
   - Label connections

4. **Document your build**
   - Take photos during assembly
   - Note any modifications
   - Keep wiring diagram handy

5. **Test incrementally**
   - Test each component before final assembly
   - Upload and test code early
   - Don't wait until everything is assembled

---

**Remember:** Most issues are simple connection problems or configuration mistakes. Work methodically through the checklist and you'll find the solution!
