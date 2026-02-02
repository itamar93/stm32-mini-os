#include "../drivers/led/led.h"
#include "../drivers/systick/systick.h"
#include "../drivers/uart/uart.h"
#include "../utils/string_utils.h"
#include "../drivers/st7789/st7789.h"

void delay(void) {
    systick_delay_ms(2000);
}

void print_message(const char *msg) {
    uart_send_data((uint8_t*)msg, my_strlen(msg));
}

void kmain(void) {
    ST7789_Init();
    
    // Fill screen with white color in RGB444 format
    // RGB444: 0xFFF (white)
    uint16_t white = 0xFFF;
    
    // Draw a rectangle of white pixels (10x10) at position (10, 10)
    for (int y = 10; y < 20; y++) {
        for (int x = 10; x < 20; x++) {
            ST7789_DrawPixel(x, y, white);
        }
    }
    
    while(1) {
    }
}