CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -g -nostdlib
LDFLAGS = -T myos.ld -Wl,-Map=build/myos.map

all:
	mkdir -p build
	$(CC) $(CFLAGS) -c src/arch/stm32f407/startup.s -o build/startup.o
	$(CC) $(CFLAGS) -c src/kernel/kernel.c -o build/kernel.o
	$(CC) $(CFLAGS) $(LDFLAGS) build/startup.o build/kernel.o -o build/myos.elf
	arm-none-eabi-objcopy -O binary build/myos.elf build/myos.bin

burn:
	st-flash write build/myos.bin 0x8000000