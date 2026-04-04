# Furuta Pendulum — Sliding Mode Controller on STM32

Full-stack implementation of a Furuta (rotary inverted) pendulum control system: from MATLAB/Simulink model design, through automatic C code generation, to embedded deployment on an STM32 Nucleo-H753ZI via PlatformIO.

---

## Overview

The Furuta pendulum is an underactuated mechanical system consisting of a driven horizontal arm and a freely-swinging vertical pendulum. This project implements a **Sliding Mode Controller (SMC)** designed in MATLAB/Simulink (`staSmc` model), auto-generated as C code using Simulink Coder (R2024b), and deployed to an STM32 microcontroller using the Arduino framework via PlatformIO.

---

## Repository Structure

```
FURUTA/
├── simulink_matlab/           # MATLAB/Simulink source files
│   ├── rip.slx                # Simulink model (staSmc — SMC controller)
│   └── furuta_pendulum_control.m  # MATLAB script (parameters, tuning)
│
└── platformio_rip/            # PlatformIO embedded project (STM32)
    ├── platformio.ini         # Board: nucleo_h753zi, framework: arduino
    └── src/
        ├── main.cpp           # Encoder reading, control loop, motor driver
        ├── controller.c       # Simulink-generated SMC implementation
        ├── controller.h       # Simulink-generated controller interface
        ├── rtwtypes.h         # Simulink Coder RTW type definitions
        ├── rtw_continuous.h   # RTW continuous-time support
        └── rtw_solver.h       # RTW solver definitions
```

---

## Hardware

| Component | Details |
|---|---|
| Microcontroller | STM32 Nucleo-H753ZI |
| Motor driver | BTS7960 (RPWM, LPWM, REN, LEN) |
| Arm encoder | Quadrature, 1080 counts/rev (pins 2, 3) |
| Pendulum encoder | YT3806, 2400 counts/rev (pins 7, 10) |
| Serial baud rate | 115200 |

**Pin mapping:**

| Signal | Pin |
|---|---|
| Motor encoder A | 2 |
| Motor encoder B | 3 |
| Pendulum encoder A (YT3806) | 7 |
| Pendulum encoder B (YT3806) | 10 |
| BTS RPWM | 5 |
| BTS LPWM | 6 |
| BTS REN | 9 |
| BTS LEN | 8 |

---

## Software Stack

- **MATLAB R2024b / Simulink** — controller design and simulation
- **Simulink Coder 24.2 (R2024b)** — automatic C code generation (ERT target, ARM 10)
- **PlatformIO** — build system and firmware upload
- **Arduino framework** — HAL for STM32 peripherals
- **Encoder library** (PJRC) — quadrature decoding support

---

## Controller Design

The SMC is implemented in the Simulink model `staSmc/controller`. Key subsystems:

| Block | Description |
|---|---|
| `Filtered Derivative` | Filtered numerical differentiation of `phi` and `theta` |
| `m11`, `m12/m21`, `m22` | Inertia matrix elements of the Furuta dynamics |
| `h1`, `h2` | Coriolis/gravity vector terms |
| `det(M)` | Inertia matrix determinant (singularity guard) |

**Controller I/O:**

| Signal | Description |
|---|---|
| `phi` | Arm angle (rad), from motor encoder |
| `theta` | Pendulum angle (rad), from YT3806 encoder |
| `u` | Torque command (Nm) to motor |

Generated entry points:
- `controller_initialize()` — called once in `setup()`
- `controller_step()` — called every 1 ms in the control loop

---

## Encoder Strategy

- **YT3806 (pendulum):** interrupt-driven on both channels (`CHANGE`) — zero missed pulses, critical for pendulum accuracy.
- **Motor arm encoder:** fast polling every 50 µs — sufficient resolution for the arm.

Angles are wrapped to `[-π, π]` before being fed into the controller.

---

## Timing

| Task | Period |
|---|---|
| Motor encoder polling | 50 µs |
| Control loop (`controller_step`) | 1 ms (1 kHz) |
| Serial debug output | 200 ms |

---

## Building and Flashing

1. Install [PlatformIO](https://platformio.org/).
2. Open the `platformio_rip/` folder.
3. Build and upload:
   ```bash
   pio run --target upload
   ```
4. Monitor serial output:
   ```bash
   pio device monitor --baud 115200
   ```

Send `r` or `R` over serial to reset both encoder counts.

---

## Serial Debug Format

```
θ:<pendulum_deg>° φ:<arm_deg>° τ:<torque_Nm>Nm M:<motor_count> Y:<yt3806_count> C:<poll_cycles>
```

---

## Regenerating Controller Code

1. Open `simulink_matlab/rip.slx` in MATLAB R2024b.
2. Tune SMC parameters in `furuta_pendulum_control.m`.
3. Run **Build Model** (Ctrl+B) — Simulink Coder generates `controller.c` and `controller.h`.
4. Copy generated files into `platformio_rip/src/`.

---

## License

MIT License — see [LICENSE](LICENSE).

The auto-generated files (`controller.c`, `controller.h`, `rtwtypes.h`, `rtw_continuous.h`, `rtw_solver.h`) carry an **Academic License** as required by MathWorks Simulink Coder and are intended for educational and research use only.
