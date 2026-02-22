# 4_gfx - Drawing Text on the Screen

> *Part 4 of building a mini OS from scratch on the STM32F407.*

## From Pixels to Characters

In the last project we got the LCD working. We can fill the screen with a color and draw individual pixels. That's progress, but it's not very useful. An OS needs to display *text*.

Think about what that actually means. We have no GPU, no font rendering engine, no FreeType library. The display is just a grid of 240x320 pixels, and the only operation we have is "set this pixel to this color." So how do you draw the letter 'A'?

You draw it pixel by pixel.

![placeholder: photo of LCD displaying "HELLO FROM STM32 MINI-OS!" text](placeholder_gfx_text.jpg)

## Bitmap Fonts

The simplest approach to text rendering: store each character as a small bitmap. We use an 8x8 grid where each row is one byte. A `1` bit means foreground, a `0` bit means background.

Here's what the letter 'A' looks like in our font:

```
Row 0: 0x00  →  . . . . . . . .
Row 1: 0x18  →  . . . 1 1 . . .
Row 2: 0x24  →  . . 1 . . 1 . .
Row 3: 0x42  →  . 1 . . . . 1 .
Row 4: 0x7E  →  . 1 1 1 1 1 1 .
Row 5: 0x42  →  . 1 . . . . 1 .
Row 6: 0x42  →  . 1 . . . . 1 .
Row 7: 0x00  →  . . . . . . . .
```

That's 8 bytes. The entire A-Z alphabet plus digits and a few symbols fits in under 400 bytes of Flash. No filesystem, no font files.

## Storing the Font Array

We use C99 designated initializers so each glyph sits at its ASCII index:

```c
const uint8_t FONTS8X8[][FONT_8X8_HEIGHT] = {
    [' ']  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['!']  = {0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x18, 0x00},
    ['0']  = {0x00, 0x3C, 0x66, 0x6E, 0x7E, 0x76, 0x3C, 0x00},
    ['A']  = {0x00, 0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x00},
    // ... B through Z
};
```

The nice thing about designated initializers: `FONTS8X8['A']` gives you the glyph for 'A' directly. No offset math, no lookup table. The compiler fills gaps between entries with zeros, so unsupported characters just render as blank.

We also define a `Font_TypeDef` struct to keep the font metadata together:

```c
typedef struct {
    const uint8_t *font;
    uint32_t width;
    uint32_t height;
} Font_TypeDef;

const Font_TypeDef FONT_8X8 = {
    .font = (const uint8_t *)FONTS8X8,
    .width = FONT_8X8_WIDTH,
    .height = FONT_8X8_HEIGHT
};
```

This makes it easy to swap fonts later. The drawing function doesn't care if it's an 8x8 or 16x16 font. It just reads `.width` and `.height`.

## Drawing a Character

To render a single character, we set the display's address window to an 8x8 region and stream pixel data row by row:

```c
uint32_t ST7789_DrawChar(uint32_t x, uint32_t y,
                         const Font_TypeDef* font, char ch,
                         uint16_t color, uint16_t bg_color) {
    const uint8_t *glyph = font->font + (uint8_t)ch * font->height;

    ST7789_ColumnAddressSet(x, x + font->width - 1);
    ST7789_RowAddressSet(y, y + font->height - 1);
    ST7789_SendCommand(ST7789_RAMWR);

    uint8_t line_buf[FONT_8X8_WIDTH * 2];
    for (uint32_t row = 0; row < font->height; row++) {
        uint8_t bits = glyph[row];
        for (uint32_t col = 0; col < font->width; col++) {
            if (bits & (0x80 >> col)) {
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

The key trick: instead of calling `DrawPixel` 64 times per character (which would be 64 separate SPI transactions with all the CS/DC toggling overhead), we set the address window once and stream the entire row as a buffer. One SPI transaction per row, 8 rows per character.

## The Application

With `DrawChar` working, rendering a string is just a loop:

```c
void kmain(void) {
    uint16_t bg_color = 0x0000;
    uint16_t text_color = 0xFFFF;
    const char *message = "HELLO FROM STM32 MINI-OS!";

    ST7789_Init();
    ST7789_FillScreen(bg_color);

    for (int i = 0; message[i] != '\0'; i++) {
        ST7789_DrawChar(i * FONT_8X8.width, 10,
                        &FONT_8X8, message[i],
                        text_color, bg_color);
    }
    while(1) {}
}
```

Each character is placed at `i * 8` pixels from the left edge. No line wrapping yet, no scrolling. Just raw text on a screen we fully control.

![placeholder: close-up photo of rendered text on the LCD](placeholder_gfx_closeup.jpg)

## What Changed in the Driver

The ST7789 driver from the previous project gained two new functions:

- `ST7789_DrawWindow()`: fills a rectangular region with a color. Uses chunked SPI transfers like `FillScreen`, but for arbitrary rectangles. Useful for clearing text regions without redrawing the whole screen.
- `ST7789_DrawChar()`: renders a single bitmap character at a given position.

Both include bounds checking and return `ST7789_ERROR_OUT_OF_BOUNDS` if you try to draw outside the display.

## What We Built (Cumulative)

| Layer | Component | New in this project |
|-------|-----------|:-------------------:|
| Platform | startup, linker, regs | |
| HAL | GPIO | |
| HAL | SysTick | |
| HAL | UART | |
| HAL | SPI | |
| Driver | LED | |
| Driver | **ST7789 (text)** | **updated** |
| Lib | string | |
| Lib | stdout | |
| Lib | **fonts** | **yes** |

## What's Next

We can display text. That's a big deal. A display that can show characters is the foundation for a console, a shell, a debugger UI. The next steps toward a real OS are getting closer: interrupt-driven I/O, a system tick, task switching. The screen will be there to show us what's happening inside.
