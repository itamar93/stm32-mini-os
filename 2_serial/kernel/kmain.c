#include "../drivers/led.h"
#include "../drivers/systick.h"

void delay(void) {
    systick_delay_ms(500);
}

void kmain(void) {
    led_init();
    while(1) {
        led_toggle();
        delay();
    }
}