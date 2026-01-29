#include "../drivers/led/led.h"
#include "../drivers/systick/systick.h"
#include "../drivers/uart/uart.h"
#include "../utils/string_utils.h"

void delay(void) {
    systick_delay_ms(2000);
}

void print_message(const char *msg) {
    uart_send_data((uint8_t*)msg, my_strlen(msg));
}

void kmain(void) {
    uart_init();
    led_init();
    print_message("3_lcd_screen\n\r");
    while(1) {
        print_message("LED HIGH\n\r");
        led_on();
        delay();
        print_message("LED LOW\n\r");
        led_off();
        delay();
    }
}