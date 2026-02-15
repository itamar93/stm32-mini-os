#ifndef MINI_OS_H
#define MINI_OS_H

/**
 * mini_os.h - Top-level convenience header for STM32 Mini-OS
 * 
 * Include this file to get access to common OS components.
 * Individual drivers and libraries can also be included directly.
 */

// Platform headers
#include "platform/stm32f407/include/regs.h"

// HAL
#include "hal/systick/systick.h"
#include "hal/gpio/gpio.h"
#include "hal/uart/uart.h"
#include "hal/spi/spi.h"

// Drivers
#include "drivers/led/led.h"
#include "drivers/st7789/st7789.h"

// Utility libraries
#include "lib/string_utils.h"
#include "lib/stdout_utils.h"

#endif /* MINI_OS_H */
