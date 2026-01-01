MEMORY
{
    FLASH (RX) : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM  (RWX) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
    .text :
    {
        *(.isr_vector)
        *(.text)
        *(.rodata)
    } > FLASH

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _sdata = .;
        *(.data)
    } > SRAM

    .bss :
    {
        *(.bss)
    } > SRAM
}