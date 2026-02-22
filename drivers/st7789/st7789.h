#ifndef ST7789_H
#define ST7789_H

#include "../../hal/spi/spi.h"
#include "../../lib/inc/fonts.h"

#define ST7789_WIDTH 240
#define ST7789_HEIGHT 320
#define ST7789_GPIO_STRUCT GPIOC
#define ST7789_GPIO_PORT GPIO_PORT_C
#define ST7789_BL_PIN     4
#define ST7789_RESET_PIN  5
#define ST7789_CS_PIN     6
#define ST7789_DC_PIN     7
// ST7789 Commands
#define ST7789_SLPOUT 0x11
#define ST7789_COLMOD 0x3A
#define ST7789_COLMOD_RGB565 0x05
#define ST7789_MADCTL 0x36
#define ST7789_MADCTL_Default 0x60
#define ST7789_DISPON 0x29
#define ST7789_CASET  0x2A
#define ST7789_RASET  0x2B
#define ST7789_RAMWR  0x2C
#define ST7789_INVON  0x21
// Error codes
#define ST7789_ERROR_OUT_OF_BOUNDS 1
#define ST7789_SUCCESS 0

void ST7789_Init(void);
uint32_t ST7789_DrawPixel(uint32_t x, uint32_t y, uint16_t color);
void ST7789_FillScreen(uint16_t color);
uint32_t ST7789_DrawWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);
uint32_t ST7789_DrawChar(uint32_t x, uint32_t y, const Font_TypeDef* font, char ch, uint16_t color, uint16_t bg_color);

#endif // ST7789_H