#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

#define FONT_12X12_WIDTH 12
#define FONT_12X12_HEIGHT 12

typedef struct {
    const uint16_t *font;
    uint32_t width;
    uint32_t height;
} Font_TypeDef;

extern const Font_TypeDef FONT_12X12;

#endif // FONTS_H