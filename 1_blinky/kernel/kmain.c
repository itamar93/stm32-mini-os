#include "../drivers/led.h"

void delay(void) {
    for(int i = 0; i < 1000000; i++) {
        __asm("nop");
    }
}

void kmain(void) {
    led_init();
    while(1) {
        led_toggle();
        delay();
    }
}