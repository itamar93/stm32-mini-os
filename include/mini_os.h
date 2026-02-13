#ifndef MINI_OS_H
#define MINI_OS_H

/**
 * mini_os.h - Top-level convenience header for STM32 Mini-OS
 * 
 * Include this file to get access to common OS components.
 * Individual drivers and libraries can also be included directly.
 */

// Architecture headers
#include "arch/stm32f407/include/regs.h"

// Core drivers
#include "drivers/systick/systick.h"
#include "drivers/gpio/gpio.h"
#include "drivers/led/led.h"

// Communication drivers
#include "drivers/uart/uart.h"
#include "drivers/spi/spi.h"

// Display drivers
#include "drivers/st7789/st7789.h"

// Utility libraries
#include "lib/string_utils.h"
#include "lib/stdout_utils.h"

#endif /* MINI_OS_H */
