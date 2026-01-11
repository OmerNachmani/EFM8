# EFM8 Microcontroller Labs

This repository contains lab projects for the **EFM8UB2** microcontroller, developed using **Simplicity Studio v5** with the Keil 8051 toolchain.

## Overview

These labs cover fundamental embedded systems concepts including GPIO, timers, PWM, interrupts, SPI communication, LCD displays, and motor control on the Silicon Labs EFM8 platform.

---

## Lab Contents

### Lab 2 - Introduction to EFM8
Basic introduction to the EFM8 microcontroller platform and GPIO operations.

**Contents:** Project export files (`.sls`)

---

### Lab 4 - LCD Text Display with SPI
Memory LCD display control using SPI communication.

**Features:**
- SPI interface at 1MHz
- Timer2 for CS delay timing
- Timer3 for 1ms tick generation
- Text rendering with custom font support

**Pin Configuration:**
| Pin | Function |
|-----|----------|
| P0.1 | Chip Select (Active High) |
| P0.6 | SPI Clock (SCK) |
| P1.0 | SPI MOSI |
| P1.4 | Display Enable |

**Clock:** SYSCLK 48MHz HFOSC / 2

---

### Lab 5 - DC Motor PWM Control with Joystick
Motor speed and direction control using PWM and analog joystick input.

**Features:**
- 8-bit PWM generation using PCA
- ADC-based joystick reading for speed control
- Button-controlled direction switching
- L293D H-Bridge motor driver integration

**Pin Configuration:**
| Pin | Function |
|-----|----------|
| P0.1 | PWM Output (Motor Enable) |
| P0.2 | BTN0 - Direction A |
| P0.3 | BTN1 - Direction B |
| P0.4 | Motor IN1 (Direction) |
| P0.5 | Motor IN2 (Direction) |
| P2.5 | Joystick Y-axis (ADC) |

**Motor Control Logic:**
- `IN1=1, IN2=0` → Clockwise
- `IN1=0, IN2=1` → Counter-clockwise
- `IN1=0, IN2=0` → Stop

---

### Lab 6 - DC Motor Control with L293D
Advanced motor control with joystick speed regulation.

**Features:**
- Real-time speed adjustment via joystick
- Deadzone handling for stable center position
- Button-controlled motor direction
- PWM duty cycle control (0-100%)

**Pin Configuration:**
| Pin | Function |
|-----|----------|
| P0.1 | PWM Output |
| P0.2 | BTN0 - Clockwise |
| P0.3 | BTN1 - Counter-clockwise |
| P0.4 | Motor IN1 |
| P0.5 | Motor IN2 |
| P2.5 | Joystick ADC Input |

**ADC Configuration:**
- 10-bit resolution (0-1023)
- Center position: ~512
- Deadzone: ±50 counts

---

### Lab 7 - Servo Motor Control with PCA
Precision servo motor positioning using the Programmable Counter Array (PCA).

**Features:**
- 16-bit PCA for accurate pulse generation
- Three position control (0°, 90°, 180°)
- Button-controlled position switching
- Standard servo PWM timing (0.6ms - 2.4ms pulse width)

**Pin Configuration:**
| Pin | Function |
|-----|----------|
| P0.2 | PB0 - Left/CCW |
| P0.3 | PB1 - Right/CW |
| PCA0 | Servo PWM Output |

**Servo Timing (4MHz clock):**
| Position | Pulse Width | PCA Compare Value |
|----------|-------------|-------------------|
| 0° | 0.6ms | 63136 |
| 90° | 1.5ms | 59536 |
| 180° | 2.4ms | 55936 |

---

## Project Structure

```
Lab[N]/
├── src/           # Source code (main.c, Interrupts.c, InitDevice.c)
├── inc/           # Header files and configuration
├── lib/           # BSP and peripheral libraries
└── *.hwconf       # Hardware configurator files
```

## Development Environment

| Component | Version/Details |
|-----------|-----------------|
| **IDE** | Simplicity Studio v5 |
| **Toolchain** | Keil 8051 v9.53.0 |
| **Target MCU** | EFM8UB2 (Silicon Labs) |
| **Clock** | 48MHz HFOSC |

## Getting Started

1. Install [Simplicity Studio v5](https://www.silabs.com/developers/simplicity-studio)
2. Clone this repository
3. Import the desired lab folder as an existing project
4. Connect your EFM8UB2 development board via USB
5. Build and flash the project

## Hardware Requirements

- **EFM8UB2 Starter Kit** (SLSTK2001A) or compatible board
- **Push Buttons** on P0.2 and P0.3
- **L293D Motor Driver** (Labs 5 & 6)
- **DC Motor** (Labs 5 & 6)
- **Servo Motor** (Lab 7) - Standard hobby servo
- **Joystick Module** with analog output (Labs 5 & 6)
- **Memory LCD** (Lab 4) - Included on starter kit

## Author

**Omer Nachmani**

## License

This project is licensed under the **GNU General Public License v3.0** - see the [LICENSE](LICENSE) file for details.

Copyright (C) 2026 Omer Nachmani
