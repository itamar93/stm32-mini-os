#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include "../../../arch/stm32f407/include/regs.h"

#define SYSTICK_CTRL_ENABLE    (1U << 0)
#define SYSTICK_CTRL_CLKSRC    (1U << 2)
#define SYSTICK_CTRL_COUNTFLAG (1U << 16)

#define ONE_MSEC_LOAD           16000UL  // Assuming 16 MHz clock

void systick_delay_ms(uint32_t delay);

#endif