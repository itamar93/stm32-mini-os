#include "st7789.h"
#include "../../hal/systick/systick.h"

static void ST7789_Select(void) {
    // chip select enable
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_CS_PIN, GPIO_PIN_LOW);
}

static void ST7789_Deselect(void) {
    // chip select disable
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_CS_PIN, GPIO_PIN_HIGH);
}

static void ST7789_Reset(void) {
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_RESET_PIN, GPIO_PIN_HIGH);
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_RESET_PIN, GPIO_PIN_LOW);
    systick_delay_us(10); // >= 10us
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_RESET_PIN, GPIO_PIN_HIGH);
    systick_delay_ms(120); // >= 120ms
}

static void ST7789_SendCommand(uint8_t cmd) {
    // Set DC pin low for command
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_DC_PIN, GPIO_PIN_LOW);
    // chip select enable
    ST7789_Select();
    // send command via SPI
    spi_send_data(&cmd, 1);
    // chip select disable
    ST7789_Deselect();
}

static void ST7789_SendData(uint8_t *data, uint32_t length) {
    // Set DC pin high for data
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_DC_PIN, GPIO_PIN_HIGH);
    // chip select enable
    ST7789_Select();
    // send data via SPI
    spi_send_data(data, length);
    // chip select disable
    ST7789_Deselect();
}

static void ST7789_ColumnAddressSet(uint32_t start, uint32_t end) {
    uint8_t data[4];
    data[0] = (start >> 8) & 0xFF; // Start high byte
    data[1] = start & 0xFF;        // Start low byte
    data[2] = (end >> 8) & 0xFF;   // End high byte
    data[3] = end & 0xFF;          // End low byte
    ST7789_SendCommand(ST7789_CASET); // Column Address Set
    ST7789_SendData(data, 4);
}

static void ST7789_RowAddressSet(uint32_t start, uint32_t end) {
    uint8_t data[4];
    data[0] = (start >> 8) & 0xFF; // Start high byte
    data[1] = start & 0xFF;        // Start low byte
    data[2] = (end >> 8) & 0xFF;   // End high byte
    data[3] = end & 0xFF;          // End low byte
    ST7789_SendCommand(ST7789_RASET); // Row Address Set
    ST7789_SendData(data, 4);
}


void ST7789_Init(void) {
    // Hardware initialization
    spi_init();
    gpio_init(ST7789_GPIO_PORT); // initialize GPIO port
    gpio_set_mode(ST7789_GPIO_STRUCT, ST7789_RESET_PIN, GPIO_MODE_OUTPUT); // configure GPIO for RESET pin
    gpio_set_mode(ST7789_GPIO_STRUCT, ST7789_CS_PIN, GPIO_MODE_OUTPUT); // configure GPIO for CS pin
    gpio_set_mode(ST7789_GPIO_STRUCT, ST7789_DC_PIN, GPIO_MODE_OUTPUT); // configure GPIO for DC pin
    gpio_set_mode(ST7789_GPIO_STRUCT, ST7789_BL_PIN, GPIO_MODE_OUTPUT);
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_BL_PIN, GPIO_PIN_HIGH);
    // cs deselect
    ST7789_Deselect();
    // reset the display
    ST7789_Reset();
    // software reset
    // ST7789_SendCommand(0x01); //TODO: add define for software reset
    // systick_delay_ms(150);
    // sleep out command
    ST7789_SendCommand(ST7789_SLPOUT);
    systick_delay_ms(120);
    // Set Color Format to RGB565
    ST7789_SendCommand(ST7789_COLMOD); // COLMOD: Interface Pixel Format
    uint8_t rgb_data = ST7789_COLMOD_RGB565;
    ST7789_SendData(&rgb_data, 1);    // 12-bit/pixel (RGB565)
    // Memory Data Access Control
    ST7789_SendCommand(ST7789_MADCTL); // MADCTL: Memory Data Access Control
    uint8_t madctl_data = ST7789_MADCTL_Default;
    ST7789_SendData(&madctl_data, 1); // Default orientation
    // inversion on
    ST7789_SendCommand(ST7789_INVON); // INVON: Inversion On
    // Turn-On Display
    ST7789_SendCommand(ST7789_DISPON); // DISPON: Display On
    // systick_delay_ms(120);
}

uint32_t ST7789_DrawPixel(uint32_t x, uint32_t y, uint16_t color) {
    if (x >= ST7789_WIDTH || y >= ST7789_HEIGHT) {
        return ST7789_ERROR_OUT_OF_BOUNDS; // Out of bounds
    }
    ST7789_ColumnAddressSet(x, x);
    ST7789_RowAddressSet(y, y);
    
    // Send RAMWR command
    ST7789_SendCommand(ST7789_RAMWR);
    
    // Byte 1: RRRRRGGG
    // Byte 2: GGGBBBBB
    uint8_t data[2];
    data[0] = (uint8_t)((color >> 8) & 0xFF); // High byte
    data[1] = (uint8_t)(color & 0xFF);        // Low byte
    ST7789_SendData(data, 2);
    return ST7789_SUCCESS;
}

void ST7789_FillScreen(uint16_t color) {
    ST7789_ColumnAddressSet(0, ST7789_WIDTH - 1);
    ST7789_RowAddressSet(0, ST7789_HEIGHT - 1);
    
    // Send RAMWR command
    ST7789_SendCommand(ST7789_RAMWR);
    
    // Prepare a buffer of pixel block (64 pixels at a time)
    uint8_t data[128]; // 64 pixels * 2 bytes/pixel
    for (uint32_t i = 0; i < 64; i++) {
        data[2*i] = (uint8_t)((color >> 8) & 0xFF); // High byte
        data[2*i + 1] = (uint8_t)(color & 0xFF);   // Low byte
    }
    
    // Send pixel data in chunks until the entire screen is filled
    for (uint32_t i = 0; i < (ST7789_WIDTH * ST7789_HEIGHT) / 64; i++) {
        ST7789_SendData(data, sizeof(data));
    }
}

uint32_t ST7789_DrawWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color) {
    if (x + width > ST7789_WIDTH || y + height > ST7789_HEIGHT) {
        return ST7789_ERROR_OUT_OF_BOUNDS; // Out of bounds
    }
    ST7789_ColumnAddressSet(x, x + width - 1);
    ST7789_RowAddressSet(y, y + height - 1);
    
    // Send RAMWR command
    ST7789_SendCommand(ST7789_RAMWR);

    // Prepare a fixed buffer of 64 pixels (128 bytes)
    uint8_t data[128]; // 64 pixels * 2 bytes/pixel
    for (uint32_t i = 0; i < 64; i++) {
        data[2*i] = (uint8_t)((color >> 8) & 0xFF); // High byte
        data[2*i + 1] = (uint8_t)(color & 0xFF);   // Low byte
    }
    
    uint32_t total_pixels = width * height;
    uint32_t full_chunks = total_pixels / 64;
    uint32_t remaining_pixels = total_pixels % 64;
    
    // Send pixel data in full 64-pixel chunks
    for (uint32_t i = 0; i < full_chunks; i++) {
        ST7789_SendData(data, sizeof(data));
    }
    
    // Send remaining pixels (reuse same buffer, just send fewer bytes)
    if (remaining_pixels > 0) {
        ST7789_SendData(data, remaining_pixels * 2);
    }
    
    return ST7789_SUCCESS;
}

uint32_t ST7789_DrawChar(uint32_t x, uint32_t y, const Font_TypeDef* font, char ch, uint16_t color, uint16_t bg_color) {
    if (x + font->width > ST7789_WIDTH || y + font->height > ST7789_HEIGHT) {
        return ST7789_ERROR_OUT_OF_BOUNDS;
    }

    const uint8_t *glyph = font->font + (uint8_t)ch * font->height * font->bytes_per_row;

    uint8_t hi = (uint8_t)((color >> 8) & 0xFF);
    uint8_t lo = (uint8_t)(color & 0xFF);
    uint8_t bg_hi = (uint8_t)((bg_color >> 8) & 0xFF);
    uint8_t bg_lo = (uint8_t)(bg_color & 0xFF);

    ST7789_ColumnAddressSet(x, x + font->width - 1);
    ST7789_RowAddressSet(y, y + font->height - 1);
    ST7789_SendCommand(ST7789_RAMWR);

    uint8_t line_buf[FONT_16X16_WIDTH * 2];
    for (uint32_t row = 0; row < font->height; row++) {
        const uint8_t *row_data = glyph + row * font->bytes_per_row;
        for (uint32_t col = 0; col < font->width; col++) {
            uint32_t byte_idx = col / 8;
            uint32_t bit_idx = 7 - (col % 8);
            if (row_data[byte_idx] & (1U << bit_idx)) {
                line_buf[col * 2]     = hi;
                line_buf[col * 2 + 1] = lo;
            } else {
                line_buf[col * 2]     = bg_hi;
                line_buf[col * 2 + 1] = bg_lo;
            }
        }
        ST7789_SendData(line_buf, font->width * 2);
    }
    return ST7789_SUCCESS;
}
