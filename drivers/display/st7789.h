#ifndef ST7789_H
#define ST7789_H

#include "../../hal/spi/spi.h"
#include "../../hal/systick/systick.h"

#define ST7789_WIDTH 320
#define ST7789_HEIGHT 240
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

void ST7789_Init(void);
void ST7789_SetWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
void ST7789_WritePixelData(uint8_t *data, uint32_t length);
// void ST7789_DrawPixel(uint32_t x, uint32_t y, uint16_t color);
// void ST7789_FillScreen(uint16_t color);
// void ST7789_DrawWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);
// void ST7789_DrawChar(uint32_t x, uint32_t y, const Font_TypeDef* font, char ch, uint16_t color, uint16_t bg_color, uint32_t *error);

#endif // ST7789_H