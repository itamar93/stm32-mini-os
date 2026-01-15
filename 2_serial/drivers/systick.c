#include "systick.h"

void systick_delay_ms(uint32_t delay) {
    SYSTICK_RELOAD = ONE_MSEC_LOAD - 1; // Load value for 1 ms
    SYSTICK_CURRENT = 0;                 // Clear current value
    SYSTICK_CTRL = SYSTICK_CTRL_CLKSRC;                // Use processor clock
    SYSTICK_CTRL |= SYSTICK_CTRL_ENABLE; // Enable SysTick

    for (uint32_t i = 0; i < delay; i++) {
        // Wait until the COUNTFLAG is set
        while ((SYSTICK_CTRL & SYSTICK_CTRL_COUNTFLAG) == 0);
    }

    SYSTICK_CTRL = 0; // Disable SysTick
}