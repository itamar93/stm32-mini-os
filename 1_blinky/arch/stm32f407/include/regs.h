#ifndef REGS_H
#define REGS_H

#include <stdint.h>

/* Peripherals Base */
#define PERIPH_BASE         0x40000000UL

/* AHB1 */
#define AHB1PERIPH_OFFSET  0x20000UL
#define AHB1PERIPH_BASE    (PERIPH_BASE + AHB1PERIPH_OFFSET)

/* RCC Registers */
#define RCC_OFFSET         0x3800UL
#define RCC_BASE           (AHB1PERIPH_BASE + RCC_OFFSET)

#define RCC_AHB1ENR_OFFSET 0x30UL
#define RCC_AHB1ENR        (*((volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR_OFFSET)))

/* GPIOD Registers */
#define GPIOD_OFFSET       0xC00UL
#define GPIOD_BASE         (AHB1PERIPH_BASE + GPIOD_OFFSET)

#define GPIOD_MODER        (*((volatile uint32_t *)(GPIOD_BASE)))
#define GPIOD_ODR_OFFSET   0x14UL
#define GPIOD_ODR          (*((volatile uint32_t *)(GPIOD_BASE + GPIOD_ODR_OFFSET)))

#define GPIOD_EN           (1U << 3)

#endif