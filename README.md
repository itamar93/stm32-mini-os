# stm32-mini-os

=== TODO ====
- setup GNU Arm Embedded Toolchain
- setup OpenOCD

=============
=== RESOURCES ===
- Reference Manuale (RM): details registers, bits' roles, register configuration, etc.
- Datasheet: provides functional overview, memory map, block diagrams, pinout, electrical characteristics, etc.
- User Manual (UM): describes the development board, which components on the board are exist and how they connected to the microcontroller
- Arm Generic User Manual: provides information on the core peripherals, such as the System Timer, Floating-Point Unit, System Control Block, Memory Protection Unit, Nested Vectored Interrupt Controller. In addition, provides information on the ISR, Execption Model, Fault handling, Power Managment, etc. 

=============
=== PERIPHERALS ===
- base address: 0x40000000
- APB: Advanced Peripheral Bus
- AHB: Advanced High-Performance Bus
- APB1, APB2: buses saved for lower bandwidth peripherals
- AHB1, AHB2: buses for high-speed data transfer peripherals
- By default, the clock to all unused peripherals is disabled, to save power (clock gating technique)
- RCC (Reset and Clock Control) is the peripheral that controlls buses' clock
- registers naming convention: peripheral + _ + register name. e.g: RCC_AHB1ENR