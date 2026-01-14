TARGET_NAME = kernel
LINKER_SCRIPT = linker.ld
BUILD_DIR = build

CC = arm-none-eabi-gcc

CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11
LDFLAGS = -nostdlib -T $(LINKER_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET_NAME).map

final: $(TARGET_NAME).elf 

*.o: *.c
	mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

$(TARGET_NAME).elf: *.o
	$(CC) $(LDFLAGS) $^ -o $(BUILD_DIR)/$@

clean:
	del -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET_NAME).elf $(BUILD_DIR)/$(TARGET_NAME).map