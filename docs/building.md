[← Previous: Setup Guide](setup.md) | [📖 Table of Contents](../README.md) | [Next →: 1_blinky](../apps/1_blinky/README.md)

---

# Build Instructions

## Prerequisites

Install the required tools:
- GNU Arm Embedded Toolchain
- OpenOCD (for flashing)

See [setup.md](setup.md) for detailed installation instructions.

## Building Applications

The repository now uses a unified build system with a single Makefile.

### Build Commands

```bash
# Build the default application (1_blinky)
make

# Build a specific application
make APP=1_blinky
make APP=2_serial
make APP=3_lcd_screen
make APP=4_gfx

# Clean build artifacts
make clean

# Build and flash to board
make flash APP=1_blinky
```

### Application Descriptions

#### 1_blinky
Simple LED blink demo. The LED toggles every 500ms.

**Usage:**
```bash
make APP=1_blinky
make flash APP=1_blinky
```

#### 2_serial
UART serial communication demo with LED control.
- Sends "Hello, World!" on startup
- Alternately turns LED on/off every 2 seconds
- Sends status messages via UART

**Usage:**
```bash
make APP=2_serial
make flash APP=2_serial
# Connect via serial terminal (9600 baud)
```

#### 3_lcd_screen
LCD display demo using ST7789 driver.
- Fills screen with a custom color
- Draws a blue 20x20 pixel square in top-left corner

**Usage:**
```bash
make APP=3_lcd_screen
make flash APP=3_lcd_screen
```

#### 4_gfx
Text rendering demo using bitmap fonts on the ST7789 LCD.
- Renders "HELLO FROM STM32-MINI-OS!" in white text on black background
- Uses 12×12 bitmap font with automatic line wrapping

**Usage:**
```bash
make APP=4_gfx
make flash APP=4_gfx
```

## Helper Scripts

The repository includes helper scripts in `scripts/`:

### Flash Script
```bash
./scripts/flash.sh [elf_file]
# Default: build/kernel.elf
```

### Debug Script
```bash
./scripts/debug.sh [elf_file]
# Starts OpenOCD server
# Then connect with: arm-none-eabi-gdb build/kernel.elf
# In GDB: target remote :3333
```

## OpenOCD Configuration

A default OpenOCD configuration is provided in `cfg/openocd.cfg` for the STM32F407 Discovery board with ST-LINK interface.

## Directory Structure

```
.
├── platform/          - Platform-specific code (startup, registers)
├── hal/               - Hardware Abstraction Layer (GPIO, UART, SPI, SysTick)
├── drivers/           - Device drivers (LED, ST7789 LCD)
├── lib/               - Utility libraries (string, stdout)
├── apps/              - Application entry points
│   ├── 1_blinky/
│   ├── 2_serial/
│   ├── 3_lcd_screen/
│   └── 4_gfx/
├── docs/              - Documentation
├── scripts/           - Helper scripts
├── cfg/               - Configuration files
├── Makefile           - Build system
└── linker.ld          - Linker script
```

## Troubleshooting

### Compilation Errors

If you get compilation errors, ensure:
1. ARM toolchain is properly installed and in PATH
2. You're using the correct APP name
3. All source files are present

### Flash Errors

If flashing fails:
1. Check that your ST-LINK is connected
2. Verify OpenOCD is installed
3. Check that the board is powered
4. Try: `openocd -f cfg/openocd.cfg`

---

[← Previous: Setup Guide](setup.md) | [📖 Table of Contents](../README.md) | [Next →: 1_blinky](../apps/1_blinky/README.md)
