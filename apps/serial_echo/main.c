#include "drivers/led/led.h"
#include "hal/systick/systick.h"
#include "hal/uart/uart.h"
#include "lib/string_utils.h"

void delay(void) {
    systick_delay_ms(2000);
}

void print_message(const char *msg) {
    uart_send_data((uint8_t*)msg, my_strlen(msg));
}

void kmain(void) {
    uart_init();
    led_init();
    print_message("Hello, World!\n\r");
    while(1) {
        print_message("LED ON\n\r");
        led_on();
        delay();
        print_message("LED OFF\n\r");
        led_off();
        delay();
    }
}
