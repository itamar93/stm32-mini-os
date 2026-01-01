CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -g -nostdlib
LDFLAGS = -T stm32-mini-os.ld -Wl,-Map=build/stm32-mini-os.map

all:
	mkdir -p build
	$(CC) $(CFLAGS) -c src/arch/stm32f407/startup.s -o build/startup.o
	$(CC) $(CFLAGS) -c src/kernel/kernel.c -o build/kernel.o
	$(CC) $(CFLAGS) $(LDFLAGS) build/startup.o build/kernel.o -o build/stm32-mini-os.elf
	arm-none-eabi-objcopy -O binary build/stm32-mini-os.elf build/stm32-mini-os.bin

burn:
	st-flash write build/stm32-mini-os.bin 0x8000000

clean:
	rm -rf build