[← Previous: 1_blinky](../1_blinky/README.md) | [📖 Table of Contents](../../README.md) | [Next →: 3_lcd_screen](../3_lcd_screen/README.md)

---

# 2_serial - Talking to the Outside World

> *Part 2 of building a mini OS from scratch on the STM32F407.*

## We Can Blink, But We're Blind

We can blink an LED. It proves the toolchain works, the startup code is correct, and we can control hardware. But the LED only tells us one thing: on or off. If something goes wrong later, say a driver isn't initializing properly, how do we know *what* broke?

We need a way to send actual text from the microcontroller to a computer. We need **UART**.

## What is UART?

UART is one of the simplest serial protocols. Two wires: TX and RX. No clock signal — both sides just agree on a speed (baud rate) ahead of time. Data goes one bit at a time: a start bit, 8 data bits, and a stop bit.

The STM32F407 has USART2 on PA2 (TX) and PA3 (RX). The Discovery board doesn't include a built-in USB-to-serial converter, so we connect an external **USB-to-TTL adapter** to these pins. Plug the adapter into the computer, open a serial terminal like PuTTY at 9600 baud, and we're talking.

## Reusing the GPIO HAL

The GPIO HAL we wrote for the LED works here too. Same `gpio_init()`, same `gpio_set_mode()`, just different pins and a different mode. This is how a layered system takes shape: build a layer, verify it works, build the next layer on top. Our GPIO HAL is now serving two consumers — the LED driver and the UART driver.

For UART, the pins use **Alternate Function** mode instead of plain output. Each pin on the STM32 can be routed to different peripherals via an alternate function selector. PA2 and PA3 use AF7 for USART2:

```c
gpio_set_mode(GPIOA, UART2_TX_PIN, GPIO_MODE_ALTERNATE);
gpio_set_mode(GPIOA, UART2_RX_PIN, GPIO_MODE_ALTERNATE);
gpio_set_alt_function(GPIOA, UART2_TX_PIN, 7);  // AF7 = USART2
gpio_set_alt_function(GPIOA, UART2_RX_PIN, 7);
```

## Enabling the UART Clock

Same pattern as GPIO: enable the clock before touching any peripheral registers. USART2 sits on the APB1 bus:

```c
RCC->APB1ENR |= (1U << 17);  // USART2 clock enable
```

## Configuring the Baud Rate

The baud rate register (BRR) needs a divider based on the peripheral clock. At 16 MHz with 9600 baud:

```c
static uint32_t calculate_baudrate(void) {
    return (APB1_CLK_FREQ + (UART2_BAUDRATE / 2)) / UART2_BAUDRATE;
}
```

The `+ (UART2_BAUDRATE / 2)` is a rounding trick. Integer division truncates, and this gets us closer to the correct divider. At 16 MHz / 9600, we get 1667, and the USART hardware uses this to generate the correct bit timing.

## Sending Our First Byte

To transmit a byte, we wait for the TX buffer to be empty (TXE flag), then write to the data register:

```c
void uart_send_data(uint8_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        while (!(USART2->SR & UART2_TXE_BIT));  // Wait for empty
        USART2->DR = data[i];
    }
}
```

That's it. Write a byte, wait for the hardware to be ready, write the next one. The USART peripheral handles framing: start bits, stop bits, timing, all in hardware.

## Building print_message - Our `printf`

Sending raw bytes is useful, but we really want to send strings. We can't use the standard library's `strlen` — there is no standard library on bare metal. So we write our own, and wrap UART into `print_message()`:

```c
void print_message(const char *msg) {
    uart_send_data((uint8_t*)msg, my_strlen(msg));
}
```

Now `print_message("Hello, World!\n\r")` shows up on a serial terminal. This might seem trivial, but every OS needs I/O. This is ours. We built the entire path: clock enable, pin config, baud rate, byte transmission. And this function becomes **critical** when we need to debug the LCD.

## LED + Serial Combined

With UART working, we combine it with the LED driver from the previous project:

```c
void kmain(void) {
    uart_init();
    led_init();
    print_message("Hello, World!\n\r");
    while(1) {
        print_message("LED ON\n\r");
        led_on();
        delay();
        print_message("LED OFF\n\r");
        led_off();
        delay();
    }
}
```

Every 2 seconds, the LED toggles and a message goes out over serial. We can see exactly what the microcontroller is doing, in real time.

## What We Built (Cumulative)

The layers are stacking up. What started as a blink demo now has real I/O:

| Layer | Component | New in this project |
|-------|-----------|:-------------------:|
| Platform | startup, linker, regs | |
| HAL | GPIO | |
| HAL | SysTick | |
| HAL | **UART** | **yes** |
| Driver | LED | |
| Lib | **string** | **yes** |
| Lib | **stdout** | **yes** |

## What's Next

We have hardware control and serial output. Time to get ambitious. We're going to drive an **LCD screen over SPI**. New protocol, new driver, and some of the best debugging lessons of this project.

---

[← Previous: 1_blinky](../1_blinky/README.md) | [📖 Table of Contents](../../README.md) | [Next →: 3_lcd_screen](../3_lcd_screen/README.md)
