#ifndef GFX_H
#define GFX_H

#include "fonts/fonts.h"
#define GFX_WHITE 0xFFFF
#define GFX_BLACK 0x0000
enum {
    GFX_SUCCESS = 0,
    GFX_OUT_OF_BOUNDS = 1
}

typedef struct {
    void (*init)(void);
    void (*draw_window)(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);
} GFX_Driver;

void GFX_Init(const GFX_Driver *driver, uint32_t width, uint32_t height); // Initialize the graphics system
void GFX_SetCursor(uint32_t x, uint32_t y, uint32_t *status); // Set the cursor position for text rendering
void GFX_GetCursor(uint32_t *x, uint32_t *y); // Get the current cursor position
void GFX_NewLine(uint32_t *status); // Move the cursor to the beginning of the next line
void GFX_SetFont(const Font_TypeDef *font); // Set the current font for text rendering
void GFX_PutChar(char ch, uint32_t *status); // Render a single character at the current cursor position
void GFX_ClearChar(void); // Clear the character at the current cursor position
void GFX_Print(const char *str); // Render a null-terminated string starting at the current cursor position


#endif // GFX_H