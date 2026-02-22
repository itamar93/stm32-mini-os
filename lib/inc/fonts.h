#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

#define FONT_8X8_WIDTH 8
#define FONT_8X8_HEIGHT 8

#define FONT_16X16_WIDTH 16
#define FONT_16X16_HEIGHT 16
#define FONT_16X16_BYTES_PER_ROW 2

typedef struct {
    const uint8_t *font;
    uint32_t width;
    uint32_t height;
    uint32_t bytes_per_row;
} Font_TypeDef;

extern const Font_TypeDef FONT_8X8;
extern const Font_TypeDef FONT_16X16;

#endif // FONTS_H