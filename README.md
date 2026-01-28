# microbit-dsp-spectrum-analyzer

Real-time DSP spectrum analyzer on **BBC micro:bit v2 (nRF52833, ARM Cortex-M4F)**.
Samples audio (~8 kHz), applies windowing + **256-point FFT**, and renders a 5-band spectrum on the 5×5 LED matrix.
Built with **bare-metal C/C++ + FreeRTOS**, minimal vendor HAL usage, and deterministic task scheduling.

## Features (current / planned)

- Audio capture (SAADC + TIMER + PPI, ping-pong buffers)
- DSP pipeline: DC removal → Hann window → FFT → magnitude → 5-band mapping
- LED matrix visualization (spectrum / RMS modes)
- UART logging for validation (FPS, peak bin, band levels)
- Button-driven mode switching
- Host-based unit tests (Unity) for DSP + mapping logic
- clang-format + clang-tidy + CI checks

## Target Hardware

- **BBC micro:bit v2** (nRF52833, Cortex-M4F, FPU)
- Audio input:
  - Primary: built-in micro:bit v2 PDM microphone
  - Optional later: external analog microphone module → SAADC

## Architecture

Tasks (FreeRTOS):

- **AudioTask**: hardware-timed sampling into ping-pong buffers
- **DspTask**: windowing + FFT + band computation
- **DisplayTask**: maps band magnitudes to LED matrix
- **LoggerTask**: UART debug output (optional, rate-limited)

Communication uses FreeRTOS queues and/or task notifications.

## LED Matrix Refresh

The 5×5 LED matrix is refreshed using **row–column multiplexing** driven by a
dedicated **hardware timer (nRF TIMER4)**.

- TIMER4 generates a **1 kHz interrupt**
- Each interrupt scans **one row**
- A full frame is refreshed every ~5 ms
- Refresh timing is **independent of FreeRTOS scheduling**


Display refresh is handled entirely inside `LedMatrix`:
- RTOS tasks only update the frame buffer (`setPixel()`, `setFrame()`)
- No timing-critical logic runs in FreeRTOS tasks

This design avoids RTOS jitter, ensures flicker-free output under load, and
leaves other hardware timers available for audio sampling and DSP timing.
However, there is an optional ScreenRefresh  task that can be used via regular threading if desired.


## Dependencies

This repo uses git submodules for third-party dependencies:

- ARM CMSIS_5: `third_party/CMSIS_5` (5.9.0-dev-178-g55b19837f)
- FreeRTOS kernel-only: `third_party/freertos-kernel` (V10.4.0-kernel-only-801-g67f59a5f5)
- Nordic HAL (device headers): `third_party/hal_nordic` (v1.3.0-rc1-3-g8f013ea)
- Unity test framework: `third_party/unity` (v2.6.1-48-g51d2db9)

Clone with submodules:

```bash
git clone --recurse-submodules <repo>
```

Use ./dev command that will set you correctly anyway.

---

## Initial Setup

```bash
./dev setup
./dev configure
./dev build
./dev test
```

> Formatting and CI checks rely on `clang-format` being installed and available on PATH.

---

## Development Commands (`./dev`)

| Command | Description |
|------|-------------|
| `./dev setup` | Install / verify development prerequisites |
| `./dev configure` | Configure CMake build |
| `./dev build` | Build firmware |
| `./dev deploy` | Flash firmware to micro:bit |
| `./dev debugserver` | Start pyOCD GDB server |
| `./dev test` | Run host-based unit tests |
| `./dev format` | Apply clang-format |
| `./dev format-check` | CI formatting check |

---

## Build & Debug (Firmware)

```bash
./dev configure
./dev build
./dev debugserver
```

In a separate terminal:

```bash
arm-none-eabi-gdb build/firmware.elf
```

---

## Host-Based Unit Tests

```bash
cmake -S . -B build-host -G Ninja \
  -DBUILD_FIRMWARE=OFF \
  -DBUILD_TESTS=ON

cmake --build build-host
ctest --test-dir build-host --output-on-failure
```

---

## Directory Structure

```
.
├── src/
    ├── app/
    │   └── main.cpp
    ├── drivers/        # Hardware drivers (buttons, LED matrix, UART, etc.)
    ├── modules/        # Pure logic modules (hardware-independent) testable on host
    ├── dsp/            # DSP algorithms (windowing, FFT, magnitude, band mapping)
    ├── platform/       # Platform-specific code (SoC, registers, pin mapping)
    │   ├── startup.s
    │   ├── linker.ld
    └── rtos/           # RTOS integration and configuration
├── third_party/
    ├── unity              # Unit testing framework (host tests)
    ├── freertos-kernel    # RTOS kernel
    ├── hal_nordic         # Nordic device headers (nRF52833 registers)
    └── CMSIS_5            # ARM CMSIS core (Cortex-M4)
├── tests/
├── scripts/
│   ├── dev-setup.sh
│   ├── dev-test.sh
│   ├── dev-format.sh
│   └── ci-check-format.sh
├─ .github/workflows/
├── CMakeLists.txt
├── arm-gcc-toolchain.cmake
├── dev
├─ LICENSE
└── README.md
```

## Build

This repo uses **CMake** with separate targets for:
- firmware (ARM cross build)
- host unit tests (native build)

### Quick start
```bash
cmake -S . -B build-host
cmake --build build-host
ctest --test-dir build-host --output-on-failure
```

## Code Quality

### Formatting
- `clang-format` enforced via scripts and CI

### Static analysis
- `clang-tidy` on selected targets

### Unit tests
- Unity-based host tests for DSP and mapping logic

## Milestones

1. Audio sampling + RMS level display
2. FFT computation + UART spectrum output
3. LED spectrum visualization
4. Mode switching (buttons)
5. Performance polish (smoothing, peak hold)

## License

MIT License (see LICENSE).
