#ifndef LED_H
#define LED_H

#include "../../hal/gpio/gpio.h"
#define LED_GPIO_PORT GPIO_PORT_D
#define LED_PIN 12

void led_init(void);
void led_toggle(void);
void led_on(void);
void led_off(void);

#endif