#!/bin/bash
# Flash script for STM32F407 using OpenOCD

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

echo "Flashing $ELF_FILE to STM32F407..."
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $ELF_FILE verify reset exit"
