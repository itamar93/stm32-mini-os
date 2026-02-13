#include "spi.h"

static void enable_clocks(void) {
    // enable SPI1 clock
    RCC->APB2ENR |= SPI1_RCC_EN;
    // enable GPIOA clock
    gpio_init(GPIO_PORT_A);
}

static void configure_pins() {
    // configure PA5 (SCK), PA6 (MISO), PA7 (MOSI) to Alternate Function Mode
    gpio_set_mode(GPIOA, SCK_PIN, GPIO_MODE_ALTERNATE);
    gpio_set_mode(GPIOA, MISO_PIN, GPIO_MODE_ALTERNATE);
    gpio_set_mode(GPIOA, MOSI_PIN, GPIO_MODE_ALTERNATE);

    // Set Alternate Function to AF5 (SPI1)
    gpio_set_alt_function(GPIOA, SCK_PIN, 5);
    gpio_set_alt_function(GPIOA, MISO_PIN, 5);
    gpio_set_alt_function(GPIOA, MOSI_PIN, 5);

}

static void configure_spi_parameters(void) {
    // Clock phase set to first edge (bit 0)
    SPI1->CR1 &= ~(1U << 0);
    // Clock polarity low when idle (bit 1)
    SPI1->CR1 &= ~(1U << 1);
    // Master mode (bit 2)
    SPI1->CR1 |= (1U << 2);
    // Baud rate prescaler to divide by 2 (bits 5:3)
    SPI1->CR1 &= ~(7U << 3);
    SPI1->CR1 |= (0U << 3); // f_PCLK / 2
    // MSB first (bit 7)
    SPI1->CR1 &= ~(1U << 7);
    // Software slave management: enable (bit 9) and set SSI (bit 8)
    SPI1->CR1 |= (1U << 9) | (1U << 8);
    // full duplex (bit 10)
    SPI1->CR1 &= ~(1U << 10);
    // 8-bit data frame format (bit 11)
    SPI1->CR1 &= ~(1U << 11);
    // CRC calculation disabled (bit 13)
    SPI1->CR1 &= ~(1U << 13);
    // unidirecional data mode (bit 15)
    SPI1->CR1 &= ~(1U << 15);
    SPI1->CR1 |= (BIDI_MODE << 15); // bidirectional mode
    // enable SPI (bit 6) - IMPORTANT !! SHOULD BE THE LAST STEP !!
    SPI1->CR1 |= (1U << 6);
}

void spi_init(void) {
    // enable clocks
    enable_clocks();
    // configure GPIO pins for SPI
    configure_pins();
    // configure SPI parameters
    configure_spi_parameters();
}
    

void spi_send_data(uint8_t *data, uint32_t length) {
    if (BIDI_MODE) {
        SPI1->CR1 &= ~(1U << 6); // disable SPI
        // Configure for transmission
        SPI1->CR1 |= (1U << 14); // set BIDIOE for transmission
        SPI1->CR1 |= (1U << 6);  // enable SPI
    }
    for (uint32_t i = 0; i < length; i++) {
        /* wait until TXE is set */
        while (!(SPI1->SR & SPI_TXE_BIT)) {}
        /* write data to SPI1->DR */
        SPI1->DR = data[i];
    }
    /* wait until BSY flag is reset */
    while (SPI1->SR & SPI_BSY_BIT) {}
    /* wait until TXE is set */
    while (!(SPI1->SR & SPI_TXE_BIT)) {}
    /* read DR and SR to clear OVR flag; cast to void to avoid unused-variable warning */
    (void)SPI1->DR;
    (void)SPI1->SR;
}

void spi_receive_data(uint8_t *buffer, uint32_t length) {
    if (BIDI_MODE) {
        SPI1->CR1 &= ~(1U << 6); // disable SPI
        // Configure for reception
        SPI1->CR1 &= ~(1U << 14); // clear BIDIOE for reception
        SPI1->CR1 |= (1U << 6);  // enable SPI
    }
    for (uint32_t i = 0; i < length; i++) {
        // send dummy byte to generate clock
        while (!(SPI1->SR & SPI_TXE_BIT)) {}
        SPI1->DR = 0xFF;
        // wait until RXNE is set
        while (!(SPI1->SR & SPI_RXNE_BIT)) {}
        // read received data
        buffer[i] = SPI1->DR;
    }
}