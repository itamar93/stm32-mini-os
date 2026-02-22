#include "drivers/st7789/st7789.h"
#include "lib/inc/fonts.h"

void kmain(void) {
    uint16_t bg_color = 0x0000; // Black
    uint16_t text_color = 0xFFFF; // White
    const char *message = "HELLO FROM STM32 MINI-OS!";
    // Initialize the display
    ST7789_Init();
    // Fill the screen with the background color
    ST7789_FillScreen(bg_color);
    // Draw the text
    for (int i = 0; message[i] != '\0'; i++) {
        ST7789_DrawChar(i * FONT_8X8.width, 10, &FONT_8X8, message[i], text_color, bg_color);
    }
    while(1) {
        // Do nothing
    }
}