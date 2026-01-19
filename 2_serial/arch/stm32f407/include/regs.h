#ifndef REGS_H
#define REGS_H

#include <stdint.h>

/* Peripherals Base */
#define PERIPH_BASE         0x40000000UL

/* AHB1 */
#define AHB1PERIPH_OFFSET  0x20000UL
#define AHB1PERIPH_BASE    (PERIPH_BASE + AHB1PERIPH_OFFSET)

/* APB1 */
#define APB1PERIPH_OFFSET  0x00000UL
#define APB1PERIPH_BASE    (PERIPH_BASE + APB1PERIPH_OFFSET)

/* RCC Registers */
#define RCC_OFFSET         0x3800UL
#define RCC_BASE           (AHB1PERIPH_BASE + RCC_OFFSET)

#define RCC_AHB1ENR_OFFSET 0x30UL
#define RCC_AHB1ENR        (*((volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR_OFFSET)))

#define RCC_APB1ENR_OFFSET 0x40UL
#define RCC_APB1ENR        (*((volatile uint32_t *)(RCC_BASE + RCC_APB1ENR_OFFSET)))

/* GPIOA Registers */
#define GPIOA_OFFSET       0x0000UL
#define GPIOA_BASE         (AHB1PERIPH_BASE + GPIOA_OFFSET)

#define GPIOA_MODER        (*((volatile uint32_t *)(GPIOA_BASE)))

#define GPIOA_AFRL_OFFSET 0x20UL
#define GPIOA_AFRL        (*((volatile uint32_t *)(GPIOA_BASE + GPIOA_AFRL_OFFSET)))

#define GPIOA_EN           (1U << 0)

/* GPIOD Registers */
#define GPIOD_OFFSET       0xC00UL
#define GPIOD_BASE         (AHB1PERIPH_BASE + GPIOD_OFFSET)

#define GPIOD_MODER        (*((volatile uint32_t *)(GPIOD_BASE)))
#define GPIOD_ODR_OFFSET   0x14UL
#define GPIOD_ODR          (*((volatile uint32_t *)(GPIOD_BASE + GPIOD_ODR_OFFSET)))

#define GPIOD_EN           (1U << 3)

/* SysTick Registers */
#define SYSTICK_BASE       0xE000E010UL
#define SYSTICK_CTRL       (*((volatile uint32_t *)(SYSTICK_BASE + 0x0)))
#define SYSTICK_RELOAD     (*((volatile uint32_t *)(SYSTICK_BASE + 0x4)))
#define SYSTICK_CURRENT    (*((volatile uint32_t *)(SYSTICK_BASE + 0x8)))

/* USART2 Registers */
#define USART2_OFFSET      0x4400UL
#define USART2_BASE        (APB1PERIPH_BASE + USART2_OFFSET)
#define USART2_SR          (*((volatile uint32_t *)(USART2_BASE + 0x00UL)))
#define USART2_DR          (*((volatile uint32_t *)(USART2_BASE + 0x04UL)))
#define USART2_BRR         (*((volatile uint32_t *)(USART2_BASE + 0x08UL)))
#define USART2_CR1         (*((volatile uint32_t *)(USART2_BASE + 0x0CUL)))

#define UART2_RCC_EN       (1U << 17)

#endif