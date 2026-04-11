#include "gfx.h"

typedef struct {
    uint32_t screen_width;
    uint32_t screen_height;
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t bg_color;
    uint32_t fg_color;
    const Font_TypeDef *current_font;
} GFX_Context;

static GFX_Context gfx_context;

void GFX_Init(const GFX_Driver *driver, uint32_t screen_width, uint32_t screen_height) {
    // Initialize the graphics context
    gfx_context.screen_width = screen_width;
    gfx_context.screen_height = screen_height;
    gfx_context.cursor_x = 0;
    gfx_context.cursor_y = 0;
    gfx_context.bg_color = GFX_BLACK;
    gfx_context.fg_color = GFX_WHITE;
    gfx_context.current_font = &FONT_12X12; // Default font
    driver->init();
}

void GFX_SetCursor(uint32_t x, uint32_t y, ) {
    gfx_context.cursor_x = x;
    gfx_context.cursor_y = y;
}

void GFX_GetCursor(uint32_t *x, uint32_t *y) {
    if (x) *x = gfx_context.cursor_x;
    if (y) *y = gfx_context.cursor_y;
}

void GFX_NewLine(void) {
    gfx_context.cursor_x = 0;
    gfx_context.cursor_y += gfx_context.current_font->height;
}

void GFX_SetFont(const Font_TypeDef *font) {
    gfx_context.current_font = font;
}

void GFX_PutChar(char ch) {
    if ((gfx_context.cursor_x + gfx_context.current_font->width > gfx_context.screen_width) || 
        (gfx_context.cursor_y + gfx_context.current_font->height > gfx_context.screen_height)) {
        return; // Out of bounds
    }
    
}

void GFX_ClearChar(void) {
    
}

void GFX_Print(const char *str) {
    
}