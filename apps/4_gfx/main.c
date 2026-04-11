#include "drivers/display/st7789.h"
#include "lib/inc/fonts.h"

void kmain(void) {
    uint16_t bg_color = 0x0000; // Black
    uint16_t text_color = 0xFFFF; // White
    const char *message = "HELLO FROM STM32-MINI-OS!";
    uint32_t ret_val;
    // Initialize the display
    ST7789_Init();
    // Fill the screen with the background color
    ST7789_FillScreen(bg_color);
    // Draw the text
    uint32_t char_idx = 0;
    uint32_t x = 0;
    uint32_t y = 10; // Start 10 pixels from the top
    while (message[char_idx] != '\0') {
        ret_val = ST7789_DrawChar(x * FONT_12X12.width, y, &FONT_12X12, message[char_idx], text_color, bg_color);
        while (ret_val != ST7789_SUCCESS) {
            // Handle out of bounds
            x = 0; // Reset x to start of line
            y += FONT_12X12.height + 5; // Move down to next line with some spacing
            ret_val = ST7789_DrawChar(x, y, &FONT_12X12, message[char_idx], text_color, bg_color);
        }
        x += 1; 
        char_idx++;
    }
    while(1) {
        // Do nothing
    }
}