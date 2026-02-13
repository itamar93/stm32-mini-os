#!/bin/bash
# Debug script for STM32F407 using OpenOCD and GDB

if [ -z "$1" ]; then
    ELF_FILE="build/kernel.elf"
else
    ELF_FILE="$1"
fi

if [ ! -f "$ELF_FILE" ]; then
    echo "Error: $ELF_FILE not found!"
    echo "Usage: $0 [elf_file]"
    exit 1
fi

echo "Starting OpenOCD server for debugging..."
echo "In another terminal, run: arm-none-eabi-gdb $ELF_FILE"
echo "Then in GDB: target remote :3333"

openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
