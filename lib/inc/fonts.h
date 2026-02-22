#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

#define FONT_8X8_WIDTH 8
#define FONT_8X8_HEIGHT 8

typedef struct {
    const uint8_t *font;
    uint32_t width;
    uint32_t height;
} Font_TypeDef;

extern const Font_TypeDef FONT_8X8;

#endif // FONTS_H