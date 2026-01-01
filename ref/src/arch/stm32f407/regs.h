/* src/arch/stm32f407/regs.h */
#ifndef REGS_H
#define REGS_H

#include <stdint.h>

/* Base Addresses */
#define PERIPH_BASE        0x40000000U
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000U)
#define RCC_BASE           (AHB1PERIPH_BASE + 0x3800U)
#define GPIOD_BASE         (AHB1PERIPH_BASE + 0x0C00U)

/* RCC Registers (Reset & Clock Control) */
#define RCC_AHB1ENR        (*((volatile uint32_t *)(RCC_BASE + 0x30)))

/* GPIO Registers (General Purpose I/O) */
#define GPIOD_MODER        (*((volatile uint32_t *)(GPIOD_BASE + 0x00)))
#define GPIOD_ODR          (*((volatile uint32_t *)(GPIOD_BASE + 0x14)))

#endif