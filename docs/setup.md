[← Previous](../README.md) | [📖 Table of Contents](../README.md) | [Next →](building.md)

---

# Setup Guide

## Software Requirements

### GNU Arm Embedded Toolchain
Install the GNU Arm Embedded Toolchain for compiling ARM Cortex-M code:
- Download from: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
- Add the `bin/` directory to your system PATH

### OpenOCD
Install OpenOCD (Open On-Chip Debugger) for flashing and debugging:
- Download from: http://openocd.org/
- Add to your system PATH

### xPack Windows Build Tools
Provides GNU Make and other utilities (sh, rm, mkdir, etc.) needed to run standard Makefiles on Windows:
- Download from: https://xpack.github.io/windows-build-tools/
- Extract and add the `bin/` directory to your system PATH

### PuTTY
Install PuTTY for serial terminal communication:
- Download from: https://www.putty.org/

## Hardware Requirements

- STM32F407 Discovery Board
- USB-to-TTL serial adapter
- TFT LCD screen with SPI interface and integrated ST7789 controller
- PS/2 keyboard
- PS/2 breakout module
- Jumper wires
- Multimeter
- Logic analyzer (optional)

## Resources

- **Reference Manual (RM)**: Details registers, bits' roles, register configuration, etc.
- **Datasheet**: Provides functional overview, memory map, block diagrams, pinout, electrical characteristics, etc.
- **User Manual (UM)**: Describes the development board, which components on the board exist and how they are connected to the microcontroller
- **ARM Generic User Manual**: Provides information on the core peripherals, such as the System Timer, Floating-Point Unit, System Control Block, Memory Protection Unit, Nested Vectored Interrupt Controller. In addition, provides information on the ISR, Exception Model, Fault handling, Power Management, etc.

---

[← Previous](../README.md) | [📖 Table of Contents](../README.md) | [Next →](building.md)
