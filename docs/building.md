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
# Build the default application (blinky)
make

# Build a specific application
make APP=blinky
make APP=serial_echo
make APP=lcd_demo

# Clean build artifacts
make clean

# Build and flash to board
make flash APP=blinky
```

### Application Descriptions

#### blinky
Simple LED blink demo. The LED toggles every 500ms.

**Usage:**
```bash
make APP=blinky
make flash APP=blinky
```

#### serial_echo
UART serial communication demo with LED control.
- Sends "Hello, World!" on startup
- Alternately turns LED on/off every 2 seconds
- Sends status messages via UART

**Usage:**
```bash
make APP=serial_echo
make flash APP=serial_echo
# Connect via serial terminal (9600 baud)
```

#### lcd_demo
LCD display demo using ST7789 driver.
- Fills screen with a custom color
- Draws a blue 20x20 pixel square in top-left corner

**Usage:**
```bash
make APP=lcd_demo
make flash APP=lcd_demo
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
├── arch/              - Architecture-specific startup code
├── drivers/           - Hardware drivers (shared across all apps)
├── lib/              - Utility libraries (string, stdout)
├── apps/             - Application entry points
│   ├── blinky/
│   ├── serial_echo/
│   └── lcd_demo/
├── include/          - Global headers
├── docs/             - Documentation
├── scripts/          - Helper scripts
├── cfg/              - Configuration files
├── Makefile          - Build system
└── linker.ld         - Linker script
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
