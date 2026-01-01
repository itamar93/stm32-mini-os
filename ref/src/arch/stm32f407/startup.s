.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global _estack

/* Vector Table */
.section .isr_vector, "a"
.type g_pfnVectors, %object
g_pfnVectors:
    .word _estack           /* Top of Stack */
    .word Reset_Handler     /* Reset Handler */

/* Reset Handler - The first code that executes */
.section .text
.type Reset_Handler, %function
Reset_Handler:
    /* Copy .data segment from Flash to SRAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_etext
    movs r3, #0 /* Offset counter = 0 */
    b LoopCopyDataInit

CopyDataInit:
    ldr r4, [r2, r3]  /* Load from FLASH (Source + Offset) */
    str r4, [r0, r3]  /* Store to RAM (Dest + Offset) */
    adds r3, r3, #4   /* Increment offset by 4 bytes */

LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit

    /* Zero fill the .bss segment */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopFillZerobss

FillZerobss:
    str r3, [r2]
    adds r2, r2, #4

LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

    /* Jump to C Kernel */
    bl kmain
    b .                     /* Infinite Loop if kmain returns */