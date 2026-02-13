# Default application to build
APP ?= blinky

TARGET_NAME = kernel
LINKER_SCRIPT = linker.ld
BUILD_DIR = build

# Source files
SRCS  = $(wildcard arch/stm32f407/boot/*.c)
SRCS += $(wildcard drivers/*/*.c)
SRCS += $(wildcard kernel/*.c)
SRCS += $(wildcard lib/*.c)
SRCS += apps/$(APP)/main.c

# Convert source file paths to object file paths in build directory
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

CC = arm-none-eabi-gcc

# Include paths for header files
INCLUDE_DIRS = -Iarch/stm32f407/include -I. -Idrivers -Ilib

CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -g -Wall -O0 $(INCLUDE_DIRS)
LDFLAGS = -nostdlib -T $(LINKER_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET_NAME).map

all: $(BUILD_DIR)/$(TARGET_NAME).elf 

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET_NAME).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	arm-none-eabi-size $@

clean:
	rm -rf $(BUILD_DIR)

flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BUILD_DIR)/$(TARGET_NAME).elf verify reset exit"

.PHONY: all clean flash
