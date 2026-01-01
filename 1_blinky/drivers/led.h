#ifndef LED_H
#define LED_H

#include "../arch/stm32f407/include/regs.h"
#define LED_PIN 12

void led_init(void);
void led_toggle(void);
void led_on(void);
void led_off(void);

#endif