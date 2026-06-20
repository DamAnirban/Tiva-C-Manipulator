# Servo Control — Test Programs

Standalone sweep programs used during development to validate PWM configuration and servo response for each port independently.

## Files

| File | Description |
|---|---|
| `servo-sweep-Port-C.c` | Sweeps a single servo on PC4 (M0PWM6) from 0° to 180° and back |
| `servo-sweep-Port-E.c` | Sweeps a single servo on PE4 (M0PWM4) from 0° to 180° and back |
| `servo-sweep-port-F.c` | Sweeps a single servo on PF2 (M1PWM6) from 0° to 180° and back |
| `servo-sweep-3/servo-sweep-3.c` | Sweeps all three joints sequentially with per-joint angle limits |

## PWM Configuration

All programs configure the Tiva PWM peripheral to generate a standard 50 Hz servo signal:

- System clock: 80 MHz (PLL, SYSDIV_5)
- PWM clock: 80 MHz ÷ 16 = 2.5 MHz
- PWM period: 50000 ticks = 20 ms = 50 Hz

Pulse width to angle mapping:

| Pulse | Ticks | Angle |
|---|---|---|
| 0.5 ms | 1250 | 0° |
| 1.45 ms | 3625 | 90° |
| 2.4 ms | 6000 | 180° |

These are used as building blocks for the full ADC-mapped arm in the `ADC mapped 3DOF robotic arm/` folder.
