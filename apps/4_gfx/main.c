#include "drivers/st7789/st7789.h"
#include "lib/inc/fonts.h"

void kmain(void) {
    uint16_t bg_color = 0x0000; // Black
    uint16_t text_color = 0xFFFF; // White
    // Initialize the display
    ST7789_Init();
    // Fill the screen with the background color
    ST7789_FillScreen(bg_color);
    // Draw the text
    ST7789_DrawChar(10, 10, &FONT_8X8, 'H', text_color, bg_color);
    // Draw the text
    ST7789_DrawChar(10, 20, &FONT_8X8, 'E', text_color, bg_color);
    // Draw the text
    ST7789_DrawChar(10, 30, &FONT_8X8, 'L', text_color, bg_color);
    // Draw the text
    ST7789_DrawChar(10, 40, &FONT_8X8, 'L', text_color, bg_color);
    // Draw the text
    ST7789_DrawChar(10, 50, &FONT_8X8, 'O', text_color, bg_color);
    while(1) {
        // Do nothing
    }
}