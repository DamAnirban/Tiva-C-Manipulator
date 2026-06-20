# Tiva C 3-DOF Robotic Arm Manipulator

Embedded C firmware for a 3-degree-of-freedom robotic arm controlled by the **TI TM4C123GH6PM** (Tiva C LaunchPad). Three potentiometers map directly to three servo motors via ADC-to-PWM conversion, with live joint-angle feedback displayed on a 4-digit 7-segment display.

---

## Features

- Real-time ADC-to-servo mapping: twist a knob, move a joint
- Three independent joints with individual angle limits
- 4-digit multiplexed 7-segment display showing live ADC readings
- 50 Hz PWM servo control (standard hobby servo protocol)
- Modular firmware: servo drivers, ADC init, and main loop are separate compilation units

---

## Hardware

| Component | Part |
|---|---|
| Microcontroller | TI TM4C123GH6PM (ARM Cortex-M4F @ 80 MHz) |
| Development board | Tiva C LaunchPad (EK-TM4C123GXL) |
| Servos | 3× standard hobby servo (SG90 or equivalent) |
| Potentiometers | 3× 10 kΩ rotary potentiometer |
| Display | 4-digit 7-segment (common cathode) |
| IDE | TI Code Composer Studio (CCS) |

---

## Pin Mapping

### ADC Inputs (Port E)

| MCU Pin | ADC Channel | Sequencer | Controls |
|---|---|---|---|
| PE0 | AIN3 | SS3 | Joint 1 servo (PF2) |
| PE1 | AIN2 | SS1 | Joint 3 servo (PE4) |
| PE2 | AIN1 | SS2 | Joint 2 servo (PC4) |

### PWM Servo Outputs

| MCU Pin | PWM Signal | Module | Joint | Angle Range |
|---|---|---|---|---|
| PC4 | M0PWM6 | PWM0 Gen 3 | Joint 2 | 10° – 110° |
| PE4 | M0PWM4 | PWM0 Gen 2 | Joint 3 | 90° – 170° |
| PF2 | M1PWM6 | PWM1 Gen 3 | Joint 1 | 20° – 170° |

### 7-Segment Display

| Port | Function |
|---|---|
| PA4 – PA7 | Digit select (4 digits, multiplexed) |
| PB0 – PB7 | Segment data (a–g + dp) |

---

## How It Works

### ADC → Angle Conversion

The 12-bit ADC produces values in [0, 4095]. These are scaled to degrees:

```
angle = ADC_value × (180 / 4095) ≈ ADC_value × 0.04394
```

Per-joint clamping is then applied to keep each servo within its safe range.

### Angle → PWM Pulse Width

Standard hobby servos use a 50 Hz signal (20 ms period). Pulse width controls position:

| Pulse Width | Ticks (@ 2.5 MHz) | Angle |
|---|---|---|
| 0.5 ms | 1250 | 0° |
| 1.45 ms | 3625 | 90° |
| 2.4 ms | 6000 | 180° |

The conversion used in firmware:

```c
width = 1250 + (angle * 26.38);
```

The PWM clock is derived from the system clock:

```
System clock : 80 MHz  (PLL, SYSDIV_5)
PWM clock    : 80 MHz / 16 = 2.5 MHz  → period = 0.4 µs
PWM period   : 50000 ticks × 0.4 µs = 20 ms  → 50 Hz
```

### 7-Segment Display

The display is multiplexed in software: each digit is enabled in turn via Port A while Port B drives the segment pattern. The ADC raw value (0–4095) is shown as a 4-digit decimal number.

---

## Project Structure

```
Tiva-C-Manipulator/
│
├── ADC mapped 3DOF robotic arm/   ← Main project (full integration)
│   ├── main.c                     ← Main loop: read ADC → control servos → update display
│   ├── adc.c / adc.h              ← ADC0 init for 3 channels (SS1, SS2, SS3)
│   ├── servo_init.c / servo_init.h← PWM init and servo_rotate() functions
│   └── tm4c123gh6pm_startup_ccs_gcc.c  ← CCS/GCC startup file
│
├── servo-control/                 ← Servo sweep test programs
│   ├── servo-sweep-Port-C.c       ← Single servo sweep on PC4
│   ├── servo-sweep-Port-E.c       ← Single servo sweep on PE4
│   ├── servo-sweep-port-F.c       ← Single servo sweep on PF2
│   └── servo-sweep-3/
│       └── servo-sweep-3.c        ← Coordinated 3-servo sweep (all joints)
│
├── ADC/                           ← ADC development and test programs
│   ├── adc.c                      ← Single-channel ADC with 7-segment display
│   ├── 3ADC_Mixed.c               ← 3-channel ADC with 7-segment display
│   └── adc_servo_display.c        ← Single ADC channel driving one servo + display
│
└── arm/
    └── random_arm_control.c       ← Autonomous arm sweep across all three joints
```

---

## Development Environment

1. Install **[TI Code Composer Studio](https://www.ti.com/tool/CCSTUDIO)**
2. Install the **TivaWare** peripheral driver library
3. Import the project folder into CCS as an existing CCS project
4. Connect the Tiva C LaunchPad via USB
5. Build and flash using the CCS debug interface (ICDI)

The startup file (`tm4c123gh6pm_startup_ccs_gcc.c`) and linker script (`tm4c123gh6pm.lds`) are included in the main project folder.

---

## Development Progression

The firmware was built incrementally:

1. **Single servo sweep** — validated PWM timing and servo response per port
2. **3-servo coordinated sweep** — confirmed all three axes move in sequence
3. **ADC reading + 7-seg display** — validated analog input scaling and display multiplexing
4. **Full integration** — combined ADC input, servo control, and display into the final firmware

---

## License

MIT
