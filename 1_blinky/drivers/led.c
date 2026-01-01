#include "led.h"

void led_init(void) {
    // Enable Clock for GPIOD
    RCC_AHB1ENR |= GPIOD_EN;
    // Set PD12 to Output Mode (Bits 25:24 = 01)
    GPIOD_MODER |= (1U << 24); // set bit 24
    GPIOD_MODER &= ~(1U << 25); // clear bit 25
}

void led_toggle(void) {
    GPIOD_ODR ^= (1U << LED_PIN);
}

void led_on(void) {
    GPIOD_ODR |= (1U << LED_PIN);
}

void led_off(void) {
    GPIOD_ODR &= ~(1U << LED_PIN);
}
