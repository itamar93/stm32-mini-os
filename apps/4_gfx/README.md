[← Previous: 3_lcd_screen](../3_lcd_screen/README.md) | [📖 Table of Contents](../../README.md)

---

# 4_gfx - Drawing Text on the Screen

> *Part 4 of building a mini OS from scratch on the STM32F407.*

## From Pixels to Text

We can fill the screen with color and draw rectangles. But an OS needs to display *text*. Status messages, debug output, a command line — all of it requires rendering characters on screen.

This project adds a **bitmap font library** and a **character drawing function** to the ST7789 driver. Together they let us render text on the LCD, one character at a time, with automatic line wrapping.

![placeholder: photo of LCD displaying "HELLO FROM STM32-MINI-OS!" text](placeholder_gfx_text.jpg)

## Bitmap Fonts

There's no font engine on bare metal. No TrueType, no FreeType. We define each character as a grid of bits: 1 = foreground pixel, 0 = background pixel.

The font is a 12×12 bitmap. Each row of a character is stored as a `uint16_t`, where the upper 12 bits represent the pixels. Here's part of the letter 'A':

```
Row  0: 0b001111110000  →  . . 1 1 1 1 1 1 . . . .
Row  1: 0b011111111000  →  . 1 1 1 1 1 1 1 1 . . .
Row  2: 0b110000001100  →  1 1 . . . . . . 1 1 . .
Row  3: 0b110000001100  →  1 1 . . . . . . 1 1 . .
Row  4: 0b111111111100  →  1 1 1 1 1 1 1 1 1 1 . .
...
```

Each character uses 12 × 2 = 24 bytes. The entire alphabet, digits, and punctuation fit comfortably in Flash. No filesystem, no font files.

## Storing the Font Array

We use C99 designated initializers so each glyph sits at its ASCII index. Each entry is an array of 12 `uint16_t` rows:

```c
const uint16_t FONTS12X12[][FONT_12X12_HEIGHT] = {
    [' '] = { 0,0,0,0,0,0,0,0,0,0,0,0 },
    ['A'] = {
        0b001111110000,
        0b011111111000,
        0b110000001100,
        0b110000001100,
        0b110000001100,
        0b111111111100,
        0b111111111100,
        0b110000001100,
        0b110000001100,
        0b110000001100,
        0b110000001100,
        0b000000000000,
    },
    // ... B through Z, 0-9, punctuation
};
```

The nice thing about designated initializers: `FONTS12X12['A']` gives you the glyph for 'A' directly. No offset math, no lookup table. The compiler fills gaps between entries with zeros, so unsupported characters just render as blank.

A `Font_TypeDef` struct keeps the font metadata together:

```c
typedef struct {
    const uint16_t *font;
    uint32_t width;
    uint32_t height;
} Font_TypeDef;

extern const Font_TypeDef FONT_12X12;
```

The font data is `const`, so it lives in Flash and costs zero SRAM. Characters are indexed by their ASCII value — the glyph data for `'A'` lives at `font[65 * height]`. Instant lookup, no search, no hash table.

## Drawing a Character

`ST7789_DrawChar()` sets a window on the display sized to the character (12×12 pixels), then streams pixel data row by row:

```c
uint32_t ST7789_DrawChar(uint32_t x, uint32_t y,
                         const Font_TypeDef* font, char ch,
                         uint16_t color, uint16_t bg_color) {
    if (x + font->width > ST7789_WIDTH || y + font->height > ST7789_HEIGHT) {
        return ST7789_ERROR_OUT_OF_BOUNDS;
    }

    const uint16_t *glyph = font->font + (uint16_t)ch * font->height;

    ST7789_ColumnAddressSet(x, x + font->width - 1);
    ST7789_RowAddressSet(y, y + font->height - 1);
    ST7789_SendCommand(ST7789_RAMWR);

    uint8_t line_buf[FONT_12X12_WIDTH * 2];
    for (uint32_t row = 0; row < font->height; row++) {
        uint16_t bits = glyph[row];
        for (uint32_t col = 0; col < font->width; col++) {
            if (bits & (0x0800 >> col)) {
                line_buf[col * 2]     = color >> 8;
                line_buf[col * 2 + 1] = color & 0xFF;
            } else {
                line_buf[col * 2]     = bg_color >> 8;
                line_buf[col * 2 + 1] = bg_color & 0xFF;
            }
        }
        ST7789_SendData(line_buf, font->width * 2);
    }
    return ST7789_SUCCESS;
}
```

Since each row is a single `uint16_t`, bit extraction is straightforward: `0x0800 >> col` masks bit 11 down to bit 0. For each row of the glyph, it walks the bits left to right. Set bit → foreground color. Clear bit → background color. The entire row is buffered and sent in a single SPI transfer — one SPI transaction per row, 12 rows per character.

## Bounds Checking and Line Wrapping

The driver returns `ST7789_ERROR_OUT_OF_BOUNDS` if a character would exceed the screen dimensions. The application uses this to implement line wrapping:

```c
ret_val = ST7789_DrawChar(x * FONT_12X12.width, y, &FONT_12X12,
                          message[char_idx], text_color, bg_color);
while (ret_val != ST7789_SUCCESS) {
    x = 0;
    y += FONT_12X12.height + 5;  // Next line with spacing
    ret_val = ST7789_DrawChar(x, y, &FONT_12X12,
                              message[char_idx], text_color, bg_color);
}
```

When a character doesn't fit on the current line, reset x to 0 and move y down by the font height plus some spacing. Simple, but it works.

## The Application

The demo draws white text on a black screen:

```c
void kmain(void) {
    uint16_t bg_color = 0x0000;   // Black
    uint16_t text_color = 0xFFFF; // White
    const char *message = "HELLO FROM STM32-MINI-OS!";

    ST7789_Init();
    ST7789_FillScreen(bg_color);

    uint32_t char_idx = 0;
    uint32_t x = 0;
    uint32_t y = 10;
    while (message[char_idx] != '\0') {
        ret_val = ST7789_DrawChar(x * FONT_12X12.width, y, &FONT_12X12,
                                  message[char_idx], text_color, bg_color);
        while (ret_val != ST7789_SUCCESS) {
            x = 0;
            y += FONT_12X12.height + 5;
            ret_val = ST7789_DrawChar(x, y, &FONT_12X12,
                                      message[char_idx], text_color, bg_color);
        }
        x += 1;
        char_idx++;
    }
    while(1) {}
}
```

Fill the screen black, then walk through each character in the message, drawing it at the current position and advancing. If it overflows the line, wrap to the next one.

![placeholder: close-up photo of rendered text on the LCD](placeholder_gfx_closeup.jpg)

## What Changed in the Driver

The ST7789 driver from the previous project gained two new functions:

- `ST7789_DrawWindow()`: fills a rectangular region with a color. Uses chunked SPI transfers like `FillScreen`, but for arbitrary rectangles. Useful for clearing text regions without redrawing the whole screen.
- `ST7789_DrawChar()`: renders a single bitmap character at a given position.

Both include bounds checking and return `ST7789_ERROR_OUT_OF_BOUNDS` if you try to draw outside the display. `ST7789_DrawPixel()` now also returns an error code instead of `void`.

## What We Built (Cumulative)

| Layer | Component | New in this project |
|-------|-----------|:-------------------:|
| Platform | startup, linker, regs | |
| HAL | GPIO | |
| HAL | SysTick | |
| HAL | UART | |
| HAL | SPI | |
| Driver | LED | |
| Driver | **ST7789 (DrawChar, DrawWindow)** | **updated** |
| Lib | string | |
| Lib | stdout | |
| Lib | **fonts** | **yes** |

## New Files

```
lib/
├── inc/
│   └── fonts.h         - Font type definition and constants
└── src/
    └── fonts.c         - 12x12 bitmap font data (ASCII glyphs)

drivers/st7789/
├── st7789.c            - Added DrawChar(), DrawWindow(), error codes
└── st7789.h
```

## What's Next

We can render text on screen. The building blocks are in place for a real terminal: pixel control, text rendering, serial I/O, and timing. Next steps toward a mini OS:

- **Interrupts**: replace polling with interrupt-driven I/O
- **SysTick interrupt**: the heartbeat of a preemptive scheduler
- **Context switching**: save/restore registers to switch between tasks
- **A scheduler**: round-robin, priority-based, or something custom
- **A shell**: with UART input and the display, we could build an interactive console

---

[← Previous: 3_lcd_screen](../3_lcd_screen/README.md) | [📖 Table of Contents](../../README.md)
