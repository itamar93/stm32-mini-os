#include "drivers/led/led.h"
#include "hal/systick/systick.h"
#include "drivers/st7789/st7789.h"

void delay(void) {
    systick_delay_ms(2000);
}

void kmain(void) {
    ST7789_Init();
    uint16_t color = 0xfac0;
    
    ST7789_FillScreen(color);

    for (uint16_t x = 0; x < 20; x++) {
        for (uint16_t y = 0; y < 20; y++) {
            ST7789_DrawPixel(x, y, 0x001f); // Blue square
        }
    }
    
    while(1) {
    }
}
