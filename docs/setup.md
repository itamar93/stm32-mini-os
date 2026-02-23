[← Previous](../README.md) | [📖 Table of Contents](../README.md) | [Next →](building.md)

---

# Setup Guide

## Required Tools

### GNU Arm Embedded Toolchain
Install the GNU Arm Embedded Toolchain for compiling ARM Cortex-M code:
- Download from: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
- Follow installation instructions for your platform

### OpenOCD
Install OpenOCD (Open On-Chip Debugger) for flashing and debugging:
- Linux: `sudo apt-get install openocd`
- macOS: `brew install openocd`
- Windows: Download from http://openocd.org/

## Hardware Requirements

- STM32F407 Discovery Board
- ST-LINK programmer (built into Discovery board)
- USB cable

## Resources

- **Reference Manual (RM)**: Details registers, bits' roles, register configuration, etc.
- **Datasheet**: Provides functional overview, memory map, block diagrams, pinout, electrical characteristics, etc.
- **User Manual (UM)**: Describes the development board, which components on the board exist and how they are connected to the microcontroller
- **ARM Generic User Manual**: Provides information on the core peripherals, such as the System Timer, Floating-Point Unit, System Control Block, Memory Protection Unit, Nested Vectored Interrupt Controller. In addition, provides information on the ISR, Exception Model, Fault handling, Power Management, etc.

## Peripherals

- Base address: 0x40000000
- **APB**: Advanced Peripheral Bus
- **AHB**: Advanced High-Performance Bus
- **APB1, APB2**: Buses saved for lower bandwidth peripherals
- **AHB1, AHB2**: Buses for high-speed data transfer peripherals
- By default, the clock to all unused peripherals is disabled to save power (clock gating technique)
- **RCC (Reset and Clock Control)**: The peripheral that controls buses' clock
- Registers naming convention: peripheral + _ + register name. e.g: RCC_AHB1ENR

---

[← Previous](../README.md) | [📖 Table of Contents](../README.md) | [Next →](building.md)
