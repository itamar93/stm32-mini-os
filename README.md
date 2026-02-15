# stm32-mini-os

A minimal embedded operating system for STM32F407 microcontroller, demonstrating various hardware interfaces and drivers.

## Project Structure

```
stm32-mini-os/
├── platform/               # Platform-specific code (startup, registers)
├── hal/                    # Hardware Abstraction Layer (GPIO, UART, SPI, SysTick)
├── drivers/                # Device drivers (LED, ST7789 LCD)
├── lib/                    # Shared utility libraries
├── apps/                   # Example applications
│   ├── blinky/            # LED blink demo
│   ├── serial_echo/       # UART serial communication demo
│   └── lcd_demo/          # LCD display demo
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
make APP=blinky
make APP=serial_echo
make APP=lcd_demo

# Clean build artifacts
make clean
```

## Flashing

Flash the built firmware to your STM32F407 board:

```bash
make flash APP=blinky
```

## Applications

### 1. Blinky (`apps/blinky`)
A simple LED blinking application that toggles an LED every 500ms.
- Demonstrates: GPIO control, SysTick timer

### 2. Serial Echo (`apps/serial_echo`)
UART communication demo that sends messages via serial port and controls LED.
- Demonstrates: UART, GPIO, SysTick timer
- Baudrate: 9600
- Connect via: PuTTY, minicom, or any serial terminal

### 3. LCD Demo (`apps/lcd_demo`)
Display demo using ST7789 LCD driver over SPI.
- Demonstrates: SPI, LCD control, GPIO, SysTick timer
- Fills screen with color and draws a blue square

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
- **ST7789**: LCD display driver

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