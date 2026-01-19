#include "../drivers/led.h"
#include "../drivers/systick.h"
#include "../drivers/uart.h"

void delay(void) {
    systick_delay_ms(2000);
}

void kmain(void) {
    uart_init();
    led_init();
    uart_write("Hello, World!\n\r");
    while(1) {
        uart_write("LED ON\n\r");
        led_on();
        delay();
        uart_write("LED OFF\n\r");
        led_off();
        delay();
    }
}