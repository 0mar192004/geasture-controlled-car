# Circuit Diagrams and Wiring Guide

This document provides detailed wiring information for both the gesture controller and robot car.

---

## Table of Contents
1. [Gesture Controller Wiring](#gesture-controller-wiring)
2. [Robot Car Wiring](#robot-car-wiring)
3. [Pin Reference Tables](#pin-reference-tables)
4. [Power Distribution](#power-distribution)
5. [Troubleshooting Connections](#troubleshooting-connections)

---

## Gesture Controller Wiring

### Components
- Arduino Nano/Uno
- MPU6050 (GY-521)
- HC-05 Bluetooth Module
- Power supply (USB or battery)

### Schematic Overview

```
        [Power Supply]
              |
              ├─→ [Arduino Nano/Uno]
              |         |
              |         ├─→ [MPU6050] (I2C: A4/SDA, A5/SCL)
              |         |
              |         └─→ [HC-05] (Serial: TX→D0, RX→D1)
              |
              └─→ [HC-05 VCC]
```

### MPU6050 Connections

```
MPU6050 Pin    →    Arduino Pin    →    Function
─────────────────────────────────────────────────
VCC            →    5V             →    Power supply
GND            →    GND            →    Ground
SDA            →    A4 (or SDA)    →    I2C Data
SCL            →    A5 (or SCL)    →    I2C Clock
XDA            →    (not used)     →    Auxiliary I2C
XCL            →    (not used)     →    Auxiliary I2C
AD0            →    (not used)     →    Address select
INT            →    (not used)     →    Interrupt pin
```

**Notes:**
- On Arduino Uno/Nano: SDA = A4, SCL = A5
- On Arduino Mega: SDA = Pin 20, SCL = Pin 21
- Keep I2C wires short (<10cm) to reduce noise
- Default I2C address: 0x68 (0x69 if AD0 is HIGH)

### HC-05 Connections (Master Mode)

```
HC-05 Pin      →    Arduino Pin    →    Function
─────────────────────────────────────────────────
VCC            →    5V             →    Power supply
GND            →    GND            →    Ground
TXD            →    D0 (RX)        →    Transmit data
RXD            →    D1 (TX)        →    Receive data
KEY/EN         →    (not used)     →    AT mode enable
STATE          →    (not used)     →    Connection status
```

**Important:**
- HC-05 RX pin may not be 5V tolerant!
- Add voltage divider if needed:
  ```
  Arduino D1 (TX) → 1kΩ → HC-05 RXD
                         ↓
                      2kΩ
                         ↓
                       GND
  ```

### Complete Gesture Controller Wiring Table

| Component | Pin | → | Arduino | Pin |
|-----------|-----|---|---------|-----|
| **MPU6050** | VCC | → | 5V | - |
| | GND | → | GND | - |
| | SDA | → | A4 | (SDA) |
| | SCL | → | A5 | (SCL) |
| **HC-05** | VCC | → | 5V | - |
| | GND | → | GND | - |
| | TX | → | D0 | (RX) |
| | RX | → | D1 | (TX) |
| **Power** | +5V | → | USB or VIN | - |
| | GND | → | GND | - |

### Breadboard Layout (Gesture Controller)

```
         Arduino Nano
    ┌──────────────────┐
    │  USB             │
    │  [o]             │
    │                  │
    │ D1 TX ●─────────────→ HC-05 RX
    │ D0 RX ●←────────────── HC-05 TX
    │ RST               │
    │ GND ●────┬───────────→ Common GND
    │ D2       │        │
    │ D3       │        │
    │ D4       │        │
    │ D5       │        │
    │ ...      │        │
    │ A4 ●────────────────→ MPU6050 SDA
    │ A5 ●────────────────→ MPU6050 SCL
    │ 5V ●─────┼───────────→ MPU6050 VCC
    │          └───────────→ HC-05 VCC
    └──────────────────┘
```

---

## Robot Car Wiring

### Components
- Arduino Nano/Uno
- HC-05 Bluetooth Module
- L298N Motor Driver
- 2x DC Motors
- Battery pack (7.4V recommended)
- Power switch

### Schematic Overview

```
    [Battery 7.4V]
          |
       [Switch]
          |
          ├─→ [L298N Motor Driver]
          |         |
          |         ├─→ [Motor Left]
          |         └─→ [Motor Right]
          |
          └─→ [Arduino] ← [5V from L298N or USB]
                  |
                  └─→ [HC-05]
```

### L298N Motor Driver Connections

```
L298N Pin      →    Connection     →    Function
──────────────────────────────────────────────────
OUT1           →    Motor A (+)    →    Left motor positive
OUT2           →    Motor A (-)    →    Left motor negative
OUT3           →    Motor B (+)    →    Right motor positive
OUT4           →    Motor B (-)    →    Right motor negative
+12V           →    Battery (+)    →    Motor power input
GND (Power)    →    Battery (-)    →    Power ground
IN1            →    Arduino D2     →    Motor A control 1
IN2            →    Arduino D4     →    Motor A control 2
IN3            →    Arduino D7     →    Motor B control 1
IN4            →    Arduino D8     →    Motor B control 2
ENA            →    Jumper ON      →    Enable Motor A (or PWM pin)
ENB            →    Jumper ON      →    Enable Motor B (or PWM pin)
+5V (out)      →    Arduino 5V     →    Logic power (optional)
GND (Logic)    →    Arduino GND    →    Common ground
```

**Important L298N Settings:**
- **Remove 5V regulator jumper** if battery voltage >7V
- **Keep ENA/ENB jumpers ON** for full speed (remove for PWM control)
- **Common ground** between Arduino and L298N is essential

### HC-05 Connections (Slave Mode)

```
HC-05 Pin      →    Arduino Pin    →    Function
─────────────────────────────────────────────────
VCC            →    5V             →    Power supply
GND            →    GND            →    Ground
TXD            →    D0 (RX)        →    Transmit data
RXD            →    D1 (TX)        →    Receive data
KEY/EN         →    (not used)     →    AT mode enable
STATE          →    (not used)     →    Connection status
```

### Complete Robot Car Wiring Table

| Component | Pin | → | Arduino/Driver | Pin |
|-----------|-----|---|---------------|-----|
| **Battery** | + | → | Switch | - |
| **Switch** | Out | → | L298N +12V | - |
| **Battery** | - | → | L298N GND | - |
| **L298N** | IN1 | → | Arduino | D2 |
| | IN2 | → | Arduino | D4 |
| | IN3 | → | Arduino | D7 |
| | IN4 | → | Arduino | D8 |
| | GND | → | Arduino | GND |
| | 5V | → | Arduino | 5V (optional) |
| | OUT1 | → | Motor Left | + |
| | OUT2 | → | Motor Left | - |
| | OUT3 | → | Motor Right | + |
| | OUT4 | → | Motor Right | - |
| **HC-05** | VCC | → | Arduino | 5V |
| | GND | → | Arduino | GND |
| | TX | → | Arduino | D0 (RX) |
| | RX | → | Arduino | D1 (TX) |

### Physical Layout (Robot Car)

```
[Top View of Robot Chassis]

        ┌─────────────────────────┐
        │                         │
        │      [Arduino]          │
        │         [●●●]           │
        │                         │
        │  [HC-05]    [L298N]     │
        │   [●●●]     [━━━━]      │
        │                         │
   [●]  │                         │  [●]
  Motor │     [Battery Pack]      │ Motor
  Left  │        [━━━━]           │ Right
   [●]  │                         │  [●]
        │                         │
        └─────────────────────────┘
             [Caster Wheel]
```

### Detailed Motor Driver Wiring

```
        L298N Motor Driver
    ┌──────────────────────┐
    │                      │
    │  [OUT1] ●────────────┼──→ Motor Left (+)
    │  [OUT2] ●────────────┼──→ Motor Left (-)
    │                      │
    │  [OUT3] ●────────────┼──→ Motor Right (+)
    │  [OUT4] ●────────────┼──→ Motor Right (-)
    │                      │
    │  [+12V] ●←───────────┼── Battery (+) via Switch
    │  [GND]  ●←───────────┼── Battery (-)
    │                      │
    │  [IN1]  ●←───────────┼── Arduino D2
    │  [IN2]  ●←───────────┼── Arduino D4
    │  [IN3]  ●←───────────┼── Arduino D7
    │  [IN4]  ●←───────────┼── Arduino D8
    │                      │
    │  [ENA]  [═]  ← Jumper ON
    │  [ENB]  [═]  ← Jumper ON
    │                      │
    │  [+5V]  ●───────────┼──→ Arduino 5V (optional)
    │  [GND]  ●───────────┼──→ Arduino GND (required)
    │                      │
    └──────────────────────┘
```

---

## Pin Reference Tables

### Arduino Uno/Nano Pin Usage

#### Gesture Controller
| Pin | Function | Connected To |
|-----|----------|--------------|
| D0 (RX) | Serial RX | HC-05 TX |
| D1 (TX) | Serial TX | HC-05 RX |
| A4 (SDA) | I2C Data | MPU6050 SDA |
| A5 (SCL) | I2C Clock | MPU6050 SCL |
| 5V | Power | MPU6050 VCC, HC-05 VCC |
| GND | Ground | MPU6050 GND, HC-05 GND |

#### Robot Car
| Pin | Function | Connected To |
|-----|----------|--------------|
| D0 (RX) | Serial RX | HC-05 TX |
| D1 (TX) | Serial TX | HC-05 RX |
| D2 | Digital Out | L298N IN1 |
| D4 | Digital Out | L298N IN2 |
| D7 | Digital Out | L298N IN3 |
| D8 | Digital Out | L298N IN4 |
| 5V | Power | HC-05 VCC (from L298N or USB) |
| GND | Ground | HC-05 GND, L298N GND |

### L298N Motor Driver Pinout

```
          ┌───────────┐
    OUT1  │  ●     ●  │ +12V
    OUT2  │  ●     ●  │ GND
    OUT3  │  ●     ●  │ +5V (jumper)
    OUT4  │  ●        │
          │           │
    IN1   │  ●        │
    IN2   │  ●        │
    IN3   │  ●        │
    IN4   │  ●        │
          │           │
    ENA   │ [═]       │
    ENB   │ [═]       │
          │           │
    GND   │  ●        │
          └───────────┘
```

---

## Power Distribution

### Gesture Controller Power Options

**Option 1: USB Power (Recommended for testing)**
```
USB Cable → Arduino 5V → MPU6050, HC-05
```
- Pros: Simple, stable voltage
- Cons: Requires cable

**Option 2: 9V Battery**
```
9V Battery → Arduino VIN → 5V regulator → MPU6050, HC-05
```
- Pros: Portable
- Cons: Battery drains quickly, limited capacity

**Option 3: 2x 18650 Li-ion (7.4V)**
```
7.4V Battery → Arduino VIN → 5V regulator → MPU6050, HC-05
```
- Pros: Good capacity, rechargeable
- Cons: Requires proper charger

### Robot Car Power Distribution

**Recommended Configuration:**
```
Battery (7.4V)
    |
    ├──→ L298N Power Input (+12V terminal)
    |         |
    |         ├──→ Motors (via H-bridge)
    |         |
    |         └──→ 5V Regulator → Arduino 5V → HC-05
    |
    └──→ Common GND
```

**Current Requirements:**
- Arduino: ~50mA
- HC-05: ~50-100mA
- MPU6050: ~5mA
- Each Motor: 100-500mA (idle to running)
- **Total: ~300-1200mA depending on load**

**Battery Recommendations:**
| Battery Type | Voltage | Capacity | Runtime |
|--------------|---------|----------|---------|
| 4x AA NiMH | 4.8V | 2000mAh | 1-2 hours |
| 6x AA NiMH | 7.2V | 2000mAh | 2-3 hours |
| 2x 18650 | 7.4V | 2500mAh | 2-4 hours |
| LiPo 2S | 7.4V | 1500mAh | 1.5-3 hours |

---

## Wire Color Conventions

### Recommended Color Coding
- **Red:** Positive power (+)
- **Black:** Ground (-)
- **Yellow:** Signal/Data lines
- **Green:** I2C SDA
- **Blue:** I2C SCL
- **Orange:** Motor control signals
- **White:** Serial TX
- **Gray:** Serial RX

### Motor Wiring
- Motors typically have:
  - Red wire: Positive
  - Black wire: Negative
- If motors run backward, simply swap the wires

---

## Voltage Divider Circuit (HC-05 RX Protection)

If your HC-05 is not 5V tolerant on RX pin:

```
Arduino TX (5V) ──┬──[1kΩ]──┬──→ HC-05 RX (3.3V)
                  │          │
                  │      [2kΩ]
                  │          │
                  │         GND
                  
Output voltage = 5V × (2kΩ / (1kΩ + 2kΩ)) = 3.33V
```

**Component Values:**
- R1 = 1kΩ (1000Ω)
- R2 = 2kΩ (2000Ω)
- Output = ~3.3V (safe for HC-05)

---

## Troubleshooting Connections

### Testing Continuity

Use multimeter in continuity mode:
1. Power OFF all circuits
2. Test each connection end-to-end
3. Should beep if connection is good
4. Check for shorts between power and ground

### Common Wiring Mistakes

| Issue | Symptom | Check |
|-------|---------|-------|
| No power | Nothing works | Verify battery voltage, check switch |
| Arduino works, motors don't | Serial works, no movement | Check L298N power, motor connections |
| Motors work backward | Wrong direction | Swap motor wires |
| Bluetooth not pairing | Can't configure | Check TX/RX not swapped |
| MPU6050 not detected | I2C error | Verify SDA/SCL connections |
| Erratic behavior | Random resets | Check common ground |

### Voltage Check Points

With multimeter set to DC voltage:

**Gesture Controller:**
- Arduino 5V pin: Should read 4.8-5.2V
- MPU6050 VCC: Should read 4.8-5.2V
- HC-05 VCC: Should read 3.3-5.0V

**Robot Car:**
- Battery terminals: Should read >6V (7.4V nominal)
- L298N +12V input: Should match battery
- Arduino 5V pin: Should read 4.8-5.2V
- L298N 5V output: Should read 4.8-5.2V (if jumper ON)

---

## Advanced Configurations

### Adding PWM Speed Control

Connect ENA and ENB to PWM pins:
```
L298N ENA → Arduino D3 (PWM)
L298N ENB → Arduino D5 (PWM)
```
Remove ENA/ENB jumpers on L298N

**Code modification:**
```cpp
#define ENA 3
#define ENB 5

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, 200); // 0-255 speed
  analogWrite(ENB, 200);
}
```

### Adding Status LEDs

```
Arduino D13 → LED → 220Ω resistor → GND
```

### Adding Buzzer Feedback

```
Arduino D12 → Buzzer (+) → Buzzer (-) → GND
```

---

## Safety Notes

⚠️ **Electrical Safety:**
1. Double-check all connections before powering on
2. Never short power and ground
3. Use appropriate wire gauge for current
4. Add fuses for protection (recommended)
5. Ensure proper polarity on all components

⚠️ **Battery Safety:**
1. Never over-discharge Li-ion batteries (<3.0V/cell)
2. Use proper charger for battery type
3. Don't leave charging unattended
4. Store batteries safely when not in use
5. Check for physical damage before use

---

## Wire Gauge Recommendations

| Application | Current | Wire Gauge |
|-------------|---------|------------|
| Signal wires | <100mA | 22-26 AWG |
| Arduino power | <500mA | 20-22 AWG |
| Motor power | 1-2A | 18-20 AWG |
| Battery main | 2-5A | 16-18 AWG |

---

## Additional Resources

- **Arduino Pinout Diagrams:** https://www.arduino.cc/
- **L298N Datasheet:** Search for "L298N datasheet PDF"
- **MPU6050 Datasheet:** Search for "MPU6050 datasheet PDF"
- **HC-05 AT Commands:** Search for "HC-05 AT command set"

---

## Creating Your Own Diagrams

**Tools for circuit diagrams:**
- Fritzing (free, easy to use)
- KiCad (free, professional)
- EasyEDA (online, free)
- Tinkercad Circuits (online, beginner-friendly)

**Tips:**
- Take photos during assembly
- Label all wires
- Use colored wires consistently
- Document any changes you make

---

*For questions about wiring, please open an issue on GitHub with clear photos of your setup.*
