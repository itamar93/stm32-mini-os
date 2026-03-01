[← Previous: 2_serial](../2_serial/README.md) | [📖 Table of Contents](../../README.md) | [Next →: 4_gfx](../4_gfx/README.md)

---

# 3_lcd_screen - Driving an LCD Display (and Debugging Hardware)

> *Part 3 of building a mini OS from scratch on the STM32F407.*

## Time to Display Something

We can blink LEDs and send text over serial. Now let's push pixels to a screen. A 240x320 TFT LCD with an **ST7789** controller, connected over SPI.

This project adds **SPI** and the **ST7789 driver**. But honestly, the most valuable part wasn't the code — it was the debugging. Things broke in ways that had nothing to do with software, and serial output (from the previous project) is what saved us.

The end goal: this screen becomes the terminal for our mini OS. Rendering text, displaying system output, and eventually providing an interactive command line.

## Understanding SPI

SPI (Serial Peripheral Interface) is synchronous. Unlike UART, it has a clock line. The master (our STM32) drives the clock and sends data on dedicated lines:

- **SCK** (PA5): Clock
- **MOSI** (PA7): Master Out, Slave In
- **MISO** (PA6): Master In, Slave Out (not used by the display)
- **CS**: Chip Select (active low — pull it low to talk to the device)

The ST7789 also needs two extra GPIO lines:
- **DC**: Data/Command select (low = command, high = data)
- **RESET**: Hardware reset

## The SPI Driver

Same pattern as every peripheral: enable clocks, configure pins, set parameters. For SPI, the pins use Alternate Function mode (AF5 for SPI1), and then we configure the control register bit by bit:

```c
static void configure_spi_parameters(void) {
    SPI1->CR1 &= ~(1U << 0);  // Clock phase: first edge
    SPI1->CR1 &= ~(1U << 1);  // Clock polarity: low when idle
    SPI1->CR1 |= (1U << 2);   // Master mode
    SPI1->CR1 &= ~(7U << 3);  // Baud rate: f_PCLK / 2
    SPI1->CR1 &= ~(1U << 7);  // MSB first
    SPI1->CR1 |= (1U << 9) | (1U << 8);  // Software slave management
    SPI1->CR1 &= ~(1U << 11); // 8-bit data frame
    // ... more configuration ...

    // IMPORTANT: Enable SPI LAST, after all configuration is done
    SPI1->CR1 |= (1U << 6);   // Enable SPI
}
```

Notice that comment at the end. It's going to be relevant shortly.

## Sending Data Over SPI

Wait for TX empty, write a byte, repeat:

```c
void spi_send_data(uint8_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        while (!(SPI1->SR & SPI_TXE_BIT)) {}
        SPI1->DR = data[i];
    }
    while (SPI1->SR & SPI_BSY_BIT) {}  // Wait until not busy
}
```

After the loop, we wait for BSY to clear and read DR/SR to clear the overrun flag. These cleanup steps are easy to forget, and skipping them causes subtle bugs on the next transfer.

## ST7789 Initialization

The display needs a specific wake-up sequence:

1. **Hardware reset**: toggle the RESET pin
2. **Sleep Out** (`0x11`): wake the controller
3. **Wait 120ms**: the datasheet requires this
4. **Set Color Format** (`0x3A`): we use RGB565 (16-bit color)
5. **Set Memory Access Control** (`0x36`): orientation/scan direction
6. **Inversion On** (`0x21`): required for correct colors on most ST7789 modules
7. **Display On** (`0x29`)

Each command: pull DC low, select chip, send byte, deselect. Data is the same but DC high.

```c
ST7789_SendCommand(ST7789_SLPOUT);
systick_delay_ms(120);
ST7789_SendCommand(ST7789_COLMOD);
uint8_t rgb_data = 0x05;  // RGB565
ST7789_SendData(&rgb_data, 1);
```

## Drawing Pixels

To draw a pixel at (x, y), we set the column and row address window, send the RAMWR command, then send 2 bytes of color data (RGB565):

```c
uint32_t ST7789_DrawPixel(uint32_t x, uint32_t y, uint16_t color) {
    ST7789_ColumnAddressSet(x, x);
    ST7789_RowAddressSet(y, y);
    ST7789_SendCommand(ST7789_RAMWR);
    uint8_t data[2] = { color >> 8, color & 0xFF };
    ST7789_SendData(data, 2);
    return ST7789_SUCCESS;
}
```

For filling the whole screen, we set the window to the full 240x320 area and blast out pixel data in chunks of 128 bytes (64 pixels at a time):

```c
void ST7789_FillScreen(uint16_t color) {
    ST7789_ColumnAddressSet(0, 239);
    ST7789_RowAddressSet(0, 319);
    ST7789_SendCommand(ST7789_RAMWR);

    uint8_t data[128];  // 64 pixels * 2 bytes
    for (int i = 0; i < 64; i++) {
        data[2*i] = color >> 8;
        data[2*i + 1] = color & 0xFF;
    }
    for (int i = 0; i < (240 * 320) / 64; i++) {
        ST7789_SendData(data, sizeof(data));
    }
}
```

## The Application

The demo fills the screen with a color and draws a small blue square:

```c
void kmain(void) {
    ST7789_Init();
    ST7789_FillScreen(0xfac0);

    for (uint16_t x = 0; x < 20; x++)
        for (uint16_t y = 0; y < 20; y++)
            ST7789_DrawPixel(x, y, 0x001f);  // Blue

    while(1) {}
}
```

Simple. But getting here wasn't.

---

## When Things Didn't Work

### Bug #1: SPI Enabled Too Early

First flash: blank screen. Backlight was on, just no pixels, no response at all.

I knew the init sequence was correct. I'd checked it against the datasheet. So the problem had to be lower: either SPI wasn't sending data, or the data was wrong.

This is where the **UART driver from the previous project** became invaluable. I added `print_message()` calls at each SPI init step:
```c
print_message("Enabling SPI clocks...\n\r");
enable_clocks();
print_message("Configuring SPI pins...\n\r");
configure_pins();
print_message("Configuring SPI parameters...\n\r");
configure_spi_parameters();
print_message("SPI init complete\n\r");
```

Everything completed. So SPI *thought* it was initialized. But looking more carefully at the code, I found it: **SPI was enabled (bit 6 in CR1) before the rest of the configuration was done**.

The buggy code:

```c
SPI1->CR1 |= (1U << 6);   // Enable SPI - TOO EARLY!
SPI1->CR1 |= (1U << 2);   // Master mode
SPI1->CR1 |= (1U << 9);   // Software slave management
// ... more configuration after SPI was already running
```

The reference manual is clear: **SPI must not be enabled until all configuration bits are set**. Enabling it early means the peripheral starts operating with an incomplete configuration. In my case, it wasn't even in master mode yet.

Fix: move enable to the last line.

```c
// enable SPI (bit 6) - IMPORTANT !! SHOULD BE THE LAST STEP !!
SPI1->CR1 |= (1U << 6);
```

### Bug #2: Still Blank

After fixing the enable order: still blank.

Serial confirmed SPI was init'ing correctly now. The ST7789 commands were going out. Data was flowing. But nothing on the screen.

I stepped back from the software and looked at the hardware. Grabbed a **multimeter** and started probing the pins on the LCD module.
- SCK: toggling
- MOSI: data coming through
- CS, DC: switching correctly
- **VCC: nearly 0V**

The VCC pin was reading almost nothing. I traced it back: a **bad jumper wire**. It looked fine visually, seated in the breadboard, both ends plugged in. But internally, the wire had a break.

Swapped it. Screen lit up immediately.

### Takeaway

Two bugs, two completely different categories:

1. **Software bug**: SPI enabled before config was complete. Found via serial debug. Fix: one line moved.
2. **Hardware bug**: Bad jumper wire. No amount of code review finds this. You have to measure.

This is embedded development. Your code can be perfect and it still doesn't work because of a 10-cent wire. Always check the obvious physical stuff: power, ground, connections. Measure voltages. Don't assume the hardware is fine just because it looks plugged in.

## What We Built (Cumulative)

| Layer | Component | New in this project |
|-------|-----------|:-------------------:|
| Platform | startup, linker, regs | |
| HAL | GPIO | |
| HAL | SysTick | |
| HAL | UART | |
| HAL | **SPI** | **yes** |
| Driver | LED | |
| Driver | **ST7789** | **yes** |
| Lib | string | |
| Lib | stdout | |

## Final Project Structure

```
apps/3_lcd_screen/
└── main.c          - Fill screen + draw blue square

hal/spi/
├── spi.c           - SPI1 driver (init, send, receive)
└── spi.h

drivers/st7789/
├── st7789.c        - ST7789 LCD driver (init, pixel, fill)
└── st7789.h
```

## What's Next

We have a display that can draw pixels and fill rectangles. But an OS needs to show *text*: status messages, debug output, a command line. Next up: **bitmap font rendering**. We'll define each character as a grid of bits, add a `DrawChar` function to the driver, and display our first string on the screen.

---

[← Previous: 2_serial](../2_serial/README.md) | [📖 Table of Contents](../../README.md) | [Next →: 4_gfx](../4_gfx/README.md)
