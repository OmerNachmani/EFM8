# EFM8UB2 Motor Control with L293D - Complete Setup Guide

## 📋 Overview

This guide explains how to control a DC motor using:
- **EFM8UB2 SLSTK2001A** development board
- **L293D** H-bridge motor driver IC
- **Joystick** for speed control
- **Push buttons** for direction control

### Control Scheme
| Control | Function |
|---------|----------|
| **BTN0** (P0.2) | Set direction to **Clockwise** |
| **BTN1** (P0.3) | Set direction to **Counter-clockwise** |
| **Joystick Y-axis** | Speed control (center=stop, push up/down=faster) |

---

## 🔌 Hardware Wiring

### Pin Assignments Summary

| EFM8UB2 Pin | Function | Connect To |
|-------------|----------|------------|
| **P0.2** | BTN0 (built-in) | On-board button |
| **P0.3** | BTN1 (built-in) | On-board button |
| **P0.4** | Motor IN1 | L293D Pin 2 (1A) |
| **P0.5** | Motor IN2 | L293D Pin 7 (2A) |
| **P1.6** | PWM Output (CEX0) | L293D Pin 1 (1,2EN) |
| **P2.5** | Joystick Y-axis | On-board joystick |
| **GND** | Ground | L293D Pins 4,5,12,13 |
| **3.3V** | Logic supply | L293D Pin 16 (VCC1) |

### L293D Pinout Reference

```
         ┌───────┐
   1,2EN │1    16│ VCC1 (Logic +3.3V or +5V)
      1A │2    15│ 4A
      1Y │3    14│ 4Y
     GND │4    13│ GND
     GND │5    12│ GND
      2Y │6    11│ 3Y
      2A │7    10│ 3A
    VCC2 │8     9│ 3,4EN
         └───────┘
```

### Complete Wiring Diagram

```
EFM8UB2 Board                    L293D                      Motor
═══════════════                  ═════                      ═════

  P0.4 ─────────────────────────► Pin 2 (1A)
                                     │
  P0.5 ─────────────────────────► Pin 7 (2A)
                                     │
  P1.6 (PWM) ───────────────────► Pin 1 (1,2EN)
                                     │
                                  Pin 3 (1Y) ───────────────► Motor +
                                     │
                                  Pin 6 (2Y) ───────────────► Motor -
                                     │
  GND ──────────────────────────► Pins 4,5,12,13 (GND)
                                     │
  3.3V ─────────────────────────► Pin 16 (VCC1 - Logic)
                                     │
  External 6-12V ───────────────► Pin 8 (VCC2 - Motor Power)
```

### ⚠️ CRITICAL: PWM Pin Location

The PWM signal (CEX0) is routed through the **crossbar** and ends up on **P1.6** because:

1. P0.0-P0.5 are **skipped** by the crossbar
2. SPI is enabled (consumes 3 pins: P0.6→SCK, P1.1→MISO, P1.4→MOSI)
3. After SPI, CEX0 gets the next available pin: **P1.6**

**You MUST connect P1.6 to L293D EN1 (Pin 1) for speed control to work!**

---

## ⚙️ Hardware Configurator Settings

### 1. ADC Configuration (Joystick)

| Setting | Value | Notes |
|---------|-------|-------|
| ADC0 Enable | ✅ Enabled | |
| ADC Input (AMX0P) | **ADC0P.13** | This is P2.5 (Joystick Y) |
| ADC Negative Input (AMX0N) | GND | Single-ended mode |
| Justification | Right-justified | 10-bit result in ADC0H:ADC0L |
| SAR Clock Divider | 0x07 | Standard speed |

### 2. Voltage Reference (VREF)

| Setting | Value |
|---------|-------|
| Reference Source | VDD (3.3V) |
| Temperature Sensor | Disabled |
| Internal Reference Buffer | Disabled |

### 3. PCA Configuration (PWM)

| Setting | Value |
|---------|-------|
| PCA Clock Source | SYSCLK / 12 |
| PCA Mode | Run in Idle Mode |
| Channel 0 Mode | **8-bit PWM** |
| CEX0 Enable | ✅ Enabled (via XBR1) |

### 4. Port Configuration

#### Port 0

| Pin | Mode | Skip | Function |
|-----|------|------|----------|
| P0.0 | Push-Pull | ✅ Skipped | (SPI CS - optional) |
| P0.1 | Analog | ✅ Skipped | (Potentiometer) |
| P0.2 | Open-Drain | ✅ Skipped | BTN0 (direct use) |
| P0.3 | Open-Drain | ✅ Skipped | BTN1 (direct use) |
| P0.4 | Push-Pull | ✅ Skipped | **Motor IN1** |
| P0.5 | Push-Pull | ✅ Skipped | **Motor IN2** |
| P0.6 | Push-Pull | ❌ Not Skipped | (Crossbar: SPI SCK) |
| P0.7 | Push-Pull | ✅ Skipped | (Unused) |

#### Port 1

| Pin | Mode | Skip | Function |
|-----|------|------|----------|
| P1.0 | Push-Pull | ✅ Skipped | (LED) |
| P1.1 | Open-Drain | ❌ Not Skipped | (Crossbar: SPI MISO) |
| P1.2 | Open-Drain | ✅ Skipped | |
| P1.3 | Open-Drain | ✅ Skipped | |
| P1.4 | Push-Pull | ❌ Not Skipped | (Crossbar: SPI MOSI) |
| P1.5 | Analog | ✅ Skipped | |
| P1.6 | Open-Drain | ❌ Not Skipped | **Crossbar: CEX0 (PWM)** |
| P1.7 | Open-Drain | ❌ Not Skipped | |

#### Port 2

| Pin | Mode | Skip | Function |
|-----|------|------|----------|
| P2.5 | **Analog** | ✅ Skipped | **Joystick Y-axis (ADC0P.13)** |
| P2.6 | Digital | ✅ Skipped | (Joystick button) |

### 5. Crossbar Configuration (XBR0/XBR1)

| Register | Setting | Value |
|----------|---------|-------|
| XBR0 | SPI0E | ✅ Enabled (uses 3 pins) |
| XBR0 | UART0E | Disabled |
| XBR1 | XBARE | ✅ Crossbar Enabled |
| XBR1 | PCA0ME | CEX0 routed to Port pin |
| XBR1 | WEAKPUD | Weak Pullups Enabled |

---

## 🔧 Step-by-Step Configuration in Simplicity Studio

### Step 1: Open Hardware Configurator
1. Double-click `lab4a.hwconf` in your project
2. The Hardware Configurator will open

### Step 2: Configure ADC for Joystick
1. Navigate to **Peripherals → ADC0**
2. Enable ADC0
3. Set **Positive Input (AMX0P)** to `ADC0P.13` (This is P2.5)
4. Set **Negative Input (AMX0N)** to `GND`
5. Set **Justification** to `Right`

### Step 3: Configure Voltage Reference
1. Navigate to **Peripherals → VREF**
2. Set **Reference Source** to `VDD`

### Step 4: Configure PCA for PWM
1. Navigate to **Peripherals → PCA0**
2. Set **Clock Source** to `SYSCLK/12`
3. Click on **Channel 0**
4. Set **Mode** to `8-bit PWM`
5. Enable **PWM** and **ECOM** (Comparator)

### Step 5: Configure Port 0
1. Navigate to **Port I/O → Port 0**
2. Configure as shown in table above
3. **CRITICAL**: Set P0.4 and P0.5 as **Push-Pull** outputs
4. Skip P0.0-P0.5, P0.7 (leave P0.6 NOT skipped for SPI)

### Step 6: Configure Port 1
1. Navigate to **Port I/O → Port 1**
2. Set P1.6 as Digital (for PWM output)
3. Leave P1.1, P1.4, P1.6 NOT skipped

### Step 7: Configure Port 2
1. Navigate to **Port I/O → Port 2**
2. Set P2.5 as **Analog** mode (for ADC input)
3. Skip P2.5 and P2.6

### Step 8: Configure Crossbar
1. Navigate to **Port I/O → Crossbar**
2. Enable **Crossbar (XBARE)**
3. Enable **SPI0E** (SPI I/O routed to pins)
4. Set **PCA0ME** to `CEX0` (route 1 PCA channel)
5. Enable **Weak Pullups**

### Step 9: Save and Generate
1. **Save** the configurator file (Ctrl+S)
2. This automatically regenerates `InitDevice.c`

---

## 📝 Code Explanation

### ADC Reading Function
```c
uint16_t Read_ADC(void)
{
    uint16_t result;
    
    // Start conversion
    ADC0CN0 |= ADC0CN0_ADBUSY__SET;
    
    // Wait for conversion complete
    while (!(ADC0CN0 & ADC0CN0_ADINT__SET));
    
    // Clear flag and read result
    ADC0CN0 &= ~ADC0CN0_ADINT__SET;
    result = (ADC0H << 8) | ADC0L;
    
    return result;  // Returns 0-1023
}
```

### PWM Duty Cycle Function (8-bit mode)
```c
void Set_Duty_Cycle(uint8_t dc_percent)
{
    uint8_t cmp_val;
    
    if (dc_percent == 0)
        cmp_val = 0xFF;      // 0% duty (output always low)
    else if (dc_percent >= 100)
        cmp_val = 0x00;      // 100% duty (output always high)
    else
        cmp_val = (uint8_t)(256 - ((uint16_t)dc_percent * 256 / 100));
    
    PCA0CPH0 = cmp_val;
}
```

### Joystick to Speed Mapping
```c
// Joystick center = 512 (10-bit ADC)
// Deadzone = ±50 around center

if (adc_value > 562) {
    // Joystick pushed UP - map to 0-100% speed
    speed = (adc_value - 562) * 100 / (1023 - 562);
}
else if (adc_value < 462) {
    // Joystick pushed DOWN - also increase speed
    speed = (462 - adc_value) * 100 / 462;
}
else {
    // In deadzone - motor stopped
    speed = 0;
}
```

---

## 🔍 Troubleshooting

### Motor Not Spinning

| Problem | Solution |
|---------|----------|
| No power to motor | Check L293D VCC2 (Pin 8) has external motor voltage |
| PWM not reaching EN1 | Verify P1.6 is connected to L293D Pin 1 |
| Wrong ADC pin | Check AMX0P is set to ADC0P.13 (not ADC0P.4) |
| Direction pins wrong | Measure P0.4 and P0.5 with multimeter |
| Crossbar not enabled | Check XBR1 XBARE bit is set |

### Joystick Not Responding

| Problem | Solution |
|---------|----------|
| Wrong ADC channel | AMX0P must be `ADC0P.13` = `0x0D` |
| Pin not analog | P2.5 must be set to Analog mode in P2MDIN |
| Pin not skipped | P2.5 must be skipped in P2SKIP |
| ADC not enabled | Check ADC0CN0 ADEN bit |

### PWM Issues

| Problem | Solution |
|---------|----------|
| Always on/off | Check PCA0CPH0 value is being set correctly |
| No output | Verify CEX0 is enabled in XBR1 |
| Wrong pin | PWM appears on P1.6 (not P0.6) due to SPI |

### Crossbar Pin Tracing

The crossbar assigns pins in order, skipping pins marked in PxSKIP:

```
Priority: UART → SPI → SMB → CP0 → CP1 → SYSCLK → PCA

With SPI enabled and current skip settings:
1. SPI SCK  → P0.6 (first unskipped pin on P0)
2. SPI MISO → P1.1 (first unskipped pin on P1)
3. SPI MOSI → P1.4 (next unskipped pin on P1)
4. CEX0    → P1.6 (next unskipped pin after SPI)
```

---

## 📊 Quick Reference

### ADC Channel Mapping for EFM8UB2

| ADC Channel | Pin | Board Function |
|-------------|-----|----------------|
| ADC0P.0 | P0.0 | SPI CS |
| ADC0P.1 | P0.1 | Potentiometer |
| ADC0P.4 | P0.4 | (Don't use - we use for IN1) |
| **ADC0P.13** | **P2.5** | **Joystick Y-axis** |
| ADC0P.14 | P2.6 | Joystick Button |

### Motor Truth Table (L293D)

| IN1 (P0.4) | IN2 (P0.5) | EN1 (PWM) | Motor Action |
|------------|------------|-----------|--------------|
| 0 | 0 | X | Stop (coast) |
| 1 | 0 | PWM | Clockwise at PWM% speed |
| 0 | 1 | PWM | Counter-clockwise at PWM% speed |
| 1 | 1 | X | Brake (locked) |

### Key Register Values

| Register | Value | Purpose |
|----------|-------|---------|
| AMX0P | 0x0D | Select ADC0P.13 (P2.5 joystick) |
| PCA0CPM0 | 0x42 | 8-bit PWM mode, ECOM+PWM enabled |
| XBR1 | 0x41 | Crossbar enabled, CEX0 routed |

---

## ✅ Final Checklist

- [ ] AMX0P set to ADC0P.13 (0x0D) for joystick
- [ ] P2.5 configured as Analog input
- [ ] P2.5 skipped in crossbar
- [ ] PCA0 Channel 0 set to 8-bit PWM mode
- [ ] CEX0 enabled in XBR1
- [ ] Crossbar enabled (XBARE = 1)
- [ ] P0.4 and P0.5 set as Push-Pull outputs
- [ ] L293D Pin 1 (EN1) connected to **P1.6** (not P0.6!)
- [ ] L293D Pin 2 (1A) connected to P0.4
- [ ] L293D Pin 7 (2A) connected to P0.5
- [ ] L293D Pin 8 (VCC2) connected to motor power supply
- [ ] L293D Pin 16 (VCC1) connected to 3.3V logic supply
- [ ] All L293D ground pins connected to common ground
- [ ] Motor connected between Pin 3 (1Y) and Pin 6 (2Y)

---

## 📁 Project Files

| File | Purpose |
|------|---------|
| `src/main.c` | Main application with motor control logic |
| `src/InitDevice.c` | Hardware Configurator generated initialization |
| `src/Interrupts.c` | Interrupt handlers (not used in this design) |
| `lab4a.hwconf` | Hardware Configurator settings file |

---

*Last updated: January 9, 2026*
