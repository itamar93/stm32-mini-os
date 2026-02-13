#include "st7789.h"
#include "../../drivers/systick/systick.h"

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
    systick_delay_us(10); // > 10us
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_RESET_PIN, GPIO_PIN_HIGH);
    systick_delay_ms(120); // > 120ms
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

static void ST7789_ColumnAddressSet(uint16_t start, uint16_t end) {
    uint8_t data[4];
    data[0] = (start >> 8) & 0xFF; // Start high byte
    data[1] = start & 0xFF;        // Start low byte
    data[2] = (end >> 8) & 0xFF;   // End high byte
    data[3] = end & 0xFF;          // End low byte
    ST7789_SendCommand(ST7789_CASET); // Column Address Set
    ST7789_SendData(data, 4);
}

static void ST7789_RowAddressSet(uint16_t start, uint16_t end) {
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
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_BL_PIN, GPIO_PIN_LOW);
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

void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ST7789_WIDTH || y >= ST7789_HEIGHT) {
        return; // Out of bounds
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
}

void ST7789_FillScreen(uint16_t color) {
    ST7789_ColumnAddressSet(0, ST7789_WIDTH - 1);
    ST7789_RowAddressSet(0, ST7789_HEIGHT - 1);
    
    // Send RAMWR command
    ST7789_SendCommand(ST7789_RAMWR);
    
    // Prepare a buffer of pixel block (64 pixels at a time)
    uint8_t data[128]; // 64 pixels * 2 bytes/pixel
    for (int i = 0; i < 64; i++) {
        data[2*i] = (uint8_t)((color >> 8) & 0xFF); // High byte
        data[2*i + 1] = (uint8_t)(color & 0xFF);   // Low byte
    }
    
    // Send pixel data in chunks until the entire screen is filled
    for (int i = 0; i < (ST7789_WIDTH * ST7789_HEIGHT) / 64; i++) {
        ST7789_SendData(data, sizeof(data));
    }
}