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
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_RESET_PIN, GPIO_PIN_LOW);
    systick_delay_us(10);
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_RESET_PIN, GPIO_PIN_HIGH);
    systick_delay_ms(120);
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
    // reset the display
    ST7789_Reset();
    // sleep out command
    ST7789_SendCommand(ST7789_SLPOUT);
    systick_delay_ms(120);
    // Set Color Format to RGB444
    ST7789_SendCommand(ST7789_COLMOD); // COLMOD: Interface Pixel Format
    uint8_t rgb_data = ST7789_COLMOD_RGB444;
    ST7789_SendData(&rgb_data, 1);    // 12-bit/pixel (RGB444)
    // Memory Data Access Control
    ST7789_SendCommand(ST7789_MADCTL); // MADCTL: Memory Data Access Control
    uint8_t madctl_data = ST7789_MADCTL_Default;
    ST7789_SendData(&madctl_data, 1); // Default orientation
    // Turn-On Display
    ST7789_SendCommand(ST7789_DISPON); // DISPON: Display On
}

void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ST7789_WIDTH || y >= ST7789_HEIGHT) {
        return; // Out of bounds
    }
    ST7789_ColumnAddressSet(x, x);
    ST7789_RowAddressSet(y, y);
    
    // Send RAMWR command
    gpio_write_pin(ST7789_GPIO_STRUCT, ST7789_DC_PIN, GPIO_PIN_LOW);
    ST7789_Select();
    uint8_t ramwr_cmd = ST7789_RAMWR;
    spi_send_data(&ramwr_cmd, 1);
    ST7789_Deselect();
    
    // Byte 1: RRRRGGGG
    // Byte 2: BBBB0000
    uint8_t data[2];
    data[0] = (uint8_t)((color >> 4) & 0xFF); // High byte
    data[1] = (uint8_t)((color & 0xF) << 4);        // Low byte
    ST7789_SendData(data, 2);
}