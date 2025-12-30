/* src/kernel/kernel.c */
#include "../arch/stm32f407/regs.h"

void delay(int count) {
    while(count--) {
        __asm("nop");
    }
}

void kmain(void) {
    // 1. Enable Clock for GPIOD (Bit 3 in AHB1ENR)
    RCC_AHB1ENR |= (1 << 3);

    // 2. Set PD12 to Output Mode (Bits 25:24 = 01)
    // Clear bits 25:24 first, then set bit 24
    GPIOD_MODER &= ~(3 << 24); 
    GPIOD_MODER |= (1 << 24);

    while(1) {
        // 3. Toggle PD12 (Green LED)
        GPIOD_ODR ^= (1 << 12);
        
        delay(1000000); // Simple spin delay
    }
}