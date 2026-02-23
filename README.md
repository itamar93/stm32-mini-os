# stm32-mini-os

A minimal embedded operating system for STM32F407 microcontroller, demonstrating various hardware interfaces and drivers.

## Table of Contents

### Documentation
- [Setup Guide](docs/setup.md) — Tool installation and hardware requirements
- [Build Instructions](docs/building.md) — Building, flashing, and troubleshooting

### Applications
- [1_blinky](apps/1_blinky/README.md) — Blinking an LED from scratch
- [2_serial](apps/2_serial/README.md) — UART serial communication
- [3_lcd_screen](apps/3_lcd_screen/README.md) — Driving an LCD display over SPI
- [4_gfx](apps/4_gfx/README.md) — Drawing text on the screen

## Project Structure

```
stm32-mini-os/
├── platform/               # Platform-specific code (startup, registers)
├── hal/                    # Hardware Abstraction Layer (GPIO, UART, SPI, SysTick)
├── drivers/                # Device drivers (LED, ST7789 LCD)
├── lib/                    # Shared utility libraries
├── apps/                   # Example applications
│   ├── 1_blinky/          # LED blink demo
│   ├── 2_serial/          # UART serial communication demo
│   ├── 3_lcd_screen/      # LCD display demo
│   └── 4_gfx/             # Text rendering demo
├── docs/                   # Documentation
├── scripts/                # Helper scripts (flash, debug)
├── cfg/                    # Configuration files
├── Makefile               # Build system
└── linker.ld              # Linker script
```

## Building

Build any application using the `APP` variable:

```bash
# Build blinky app (default)
make

# Build specific app
make APP=1_blinky
make APP=2_serial
make APP=3_lcd_screen
make APP=4_gfx

# Clean build artifacts
make clean
```

## Flashing

Flash the built firmware to your STM32F407 board:

```bash
make flash APP=1_blinky
```

## Applications

### 1. Blinky (`apps/1_blinky`)
A simple LED blinking application that toggles an LED every 500ms.
- Demonstrates: GPIO control, SysTick timer

### 2. Serial (`apps/2_serial`)
UART communication demo that sends messages via serial port and controls LED.
- Demonstrates: UART, GPIO, SysTick timer
- Baudrate: 9600
- Connect via: PuTTY, minicom, or any serial terminal

### 3. LCD Screen (`apps/3_lcd_screen`)
Display demo using ST7789 LCD driver over SPI.
- Demonstrates: SPI, LCD control, GPIO, SysTick timer
- Fills screen with color and draws a blue square

### 4. GFX (`apps/4_gfx`)
Text rendering demo using bitmap fonts on the ST7789 LCD.
- Demonstrates: Bitmap font rendering, SPI, LCD control
- Draws white text on a black screen with line wrapping

## Hardware Requirements

- STM32F407 Discovery Board
- ST-LINK programmer (integrated on Discovery board)
- Optional: ST7789 LCD module (for lcd_demo)

## Setup

See [docs/setup.md](docs/setup.md) for detailed setup instructions including:
- GNU Arm Embedded Toolchain installation
- OpenOCD installation
- Hardware connections

## Architecture

### HAL (Hardware Abstraction Layer)
- **GPIO**: General Purpose I/O control
- **UART**: Serial communication (USART2)
- **SPI**: Serial Peripheral Interface (SPI1)
- **SysTick**: System tick timer for delays

### Drivers
- **LED**: LED control wrapper
- **ST7789**: LCD display driver (pixel, fill, window, text)

### Libraries
- **string**: Custom string utilities (`my_strlen`, etc.)
- **stdout**: Serial output wrapper (`print_message`)
- **fonts**: 12×12 bitmap font data

### Startup Flow
1. Reset_Handler (in `platform/stm32f407/boot/startup.c`)
2. Initialize `.data` section
3. Zero `.bss` section
4. Call `kmain()` from app

## Resources

- Reference Manual (RM): Register details, configuration
- Datasheet: Functional overview, memory map, pinout
- User Manual (UM): Development board details
- ARM Generic User Manual: Core peripherals documentation

## License

See [LICENSE](LICENSE) file for details.