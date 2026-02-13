#include "led.h"

void led_init(void) {
    gpio_init(LED_GPIO_PORT);
    gpio_set_mode(GPIOD, LED_PIN, GPIO_MODE_OUTPUT);
}

void led_toggle(void) {
    GPIOD->ODR ^= (GPIO_PIN_HIGH << LED_PIN);
}

void led_on(void) {
    gpio_write_pin(GPIOD, LED_PIN, GPIO_PIN_HIGH);
}

void led_off(void) {
    gpio_write_pin(GPIOD, LED_PIN, GPIO_PIN_LOW);
}
