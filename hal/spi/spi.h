#ifndef SPI_H
#define SPI_H

#include "lib/stdout_utils.h"
#include <stdint.h>
#include "../gpio/gpio.h"
#define SPI1_RCC_EN       (1U << 12)
#define SPI_TXE_BIT       (1U << 1)
#define SPI_RXNE_BIT      (1U << 0)
#define SPI_BSY_BIT       (1U << 7)
#define APB2_CLK_FREQ     160000000UL
#define SCK_PIN 5 // port A
#define MISO_PIN 6 // port A
#define MOSI_PIN 7 // port A
#define BIDI_MODE 0U

void spi_init(void);
void spi_send_data(uint8_t *data, uint32_t length);
void spi_receive_data(uint8_t *buffer, uint32_t length);

#endif // SPI_H