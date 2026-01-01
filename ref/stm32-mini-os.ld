ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .text :
    {
        *(.isr_vector)  /* Vector Table */
        *(.text)        /* Program Code */
        *(.rodata)      /* Read-only Data (e.g. const int x = 5;) */
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .data :
    {
        _sdata = .;        /* Symbol: Start of DATA in RAM */
        *(.data)           /* Put all data from C files here */
        . = ALIGN(4);      /* Make sure size is multiple of 4 bytes */
        _edata = .;        /* Symbol: End of DATA in RAM */
    } > SRAM AT> FLASH     /* > SRAM = Virtual Address (VMA)
                              AT> FLASH = Load Address (LMA) */

    .bss :
    {
        _sbss = .;
        *(.bss)
        . = ALIGN(4);
        _ebss = .;
    } > SRAM

    _estack = ORIGIN(SRAM) + LENGTH(SRAM); /* Stack Pointer at end of RAM */
}