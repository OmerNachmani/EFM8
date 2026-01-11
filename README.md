# EFM8 Microcontroller Labs

This repository contains lab projects for the **EFM8UB2** microcontroller, developed using **Simplicity Studio v5** with the Keil 8051 toolchain.

## Overview

These labs cover fundamental embedded systems concepts including GPIO, timers, PWM, interrupts, and peripheral control on the Silicon Labs EFM8 platform.

## Lab Contents

| Lab | Description |
|-----|-------------|
| **Lab 2** | Introduction to EFM8 and basic I/O |
| **Lab 4** | Timer configuration and interrupts |
| **Lab 5** | PWM generation and LED control |
| **Lab 6** | Motor control applications |
| **Lab 7** | Servo motor control using PCA (Programmable Counter Array) |

## Project Structure

Each lab folder contains:
- `src/` - Source code files (main.c, etc.)
- `inc/` - Header files and configuration
- `lib/` - Library dependencies
- `*.hwconf` - Hardware configuration files

## Development Environment

- **IDE:** Simplicity Studio v5
- **Toolchain:** Keil 8051 v9.53.0
- **Target MCU:** EFM8UB2 (Silicon Labs)

## Getting Started

1. Install [Simplicity Studio v5](https://www.silabs.com/developers/simplicity-studio)
2. Import the desired lab project (`.sls` files or project folders)
3. Connect your EFM8UB2 development board
4. Build and flash the project

## Hardware

- EFM8UB2 Starter Kit or compatible development board
- Push buttons connected to P0.2 and P0.3
- Servo motor (for Lab 7)
- LEDs and other peripherals as required by each lab

## Author

Omer Nachmani

## License

This project is for educational purposes.
