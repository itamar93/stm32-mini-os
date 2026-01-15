TARGET_NAME = kernel
LINKER_SCRIPT = linker.ld
BUILD_DIR = build

SRCS  = $(wildcard arch/stm32f407/boot/*.c)
SRCS += $(wildcard drivers/*.c)
SRCS += $(wildcard kernel/*.c)

# Convert source file paths to object file paths in build directory
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

CC = arm-none-eabi-gcc

# Include paths for header files
INCLUDE_DIRS = -Iarch/stm32f407/include -Idrivers

CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -g -Wall -O0 $(INCLUDE_DIRS)
LDFLAGS = -nostdlib -T $(LINKER_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET_NAME).map

all: $(BUILD_DIR)/$(TARGET_NAME).elf 

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET_NAME).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)

flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BUILD_DIR)/$(TARGET_NAME).elf verify reset exit"