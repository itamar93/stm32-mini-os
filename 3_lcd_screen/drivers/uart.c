#include "uart.h"

static void enable_clocks(void) {
    RCC_AHB1ENR |= GPIOA_EN;
    RCC_APB1ENR |= UART2_RCC_EN;
}

static void configure_pins() {
    // Configure TX Pin (PA2) to Alternate Function Mode (Binary 10)
    GPIOA_MODER &= ~(3U << (UART2_TX_PIN * 2)); // Clear the 2 bits
    GPIOA_MODER |=  (2U << (UART2_TX_PIN * 2)); // Set to Alternate Function Mode

    // Configure RX Pin (PA3) to Alternate Function Mode
    GPIOA_MODER &= ~(3U << (UART2_RX_PIN * 2)); // Clear the 2 bits
    GPIOA_MODER |=  (2U << (UART2_RX_PIN * 2)); // Set to Alternate Function Mode

    // Set Alternate Function to AF7 (USART2) in AF Low Register (AFRL)
    // TX Pin
    GPIOA_AFRL &= ~(0xFU << (UART2_TX_PIN * 4));     // Clear
    GPIOA_AFRL |=  (UART2_AF << (UART2_TX_PIN * 4)); // Set to 7

    // RX Pin
    GPIOA_AFRL &= ~(0xFU << (UART2_RX_PIN * 4));     // Clear
    GPIOA_AFRL |=  (UART2_AF << (UART2_RX_PIN * 4)); // Set to 7
}

static uint32_t calculate_baudrate(void) {
    // (PCLK + (Baud/2)) / Baud
    return (APB1_CLK_FREQ + (UART2_BAUDRATE / 2)) / UART2_BAUDRATE;
}

static void configure_baud_rate(void) {
    USART2_BRR = calculate_baudrate();
}

static void enable_uart(void) {
    // Enable USART, Transmit, and Receive
    USART2_CR1 = UART2_ENABLE_BIT | UART2_TE_BIT | UART2_RE_BIT;
}

void uart_init(void) {
    enable_clocks();
    configure_pins();
    configure_baud_rate();
    enable_uart();
}

void uart_send_char(uint32_t c) {
    while (!(USART2_SR & UART2_TXE_BIT)); // Wait for Empty
    USART2_DR = c;
}

uint32_t uart_receive_char(void) {
    while (!(USART2_SR & UART2_RXNE_BIT)); // Wait for Data
    return USART2_DR & 0xFF;
}

void uart_send_data(uint8_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        while (!(USART2_SR & UART2_TXE_BIT)); // Wait for Empty
        USART2_DR = data[i];
    }
}
