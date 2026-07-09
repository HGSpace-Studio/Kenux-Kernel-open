OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

MEMORY
{
    kernel (rwx) : ORIGIN = 0xffffffff80000000, LENGTH = 64M
}

SECTIONS
{
    . = 0xffffffff80000000;
    kernel_start = .;

    .text : AT(ADDR(.text) - 0xffffffff80000000)
    {
        *(.multiboot2)
        *(.text)
        *(.text.*)
        . = ALIGN(4096);
    } > kernel

    .rodata : AT(ADDR(.rodata) - 0xffffffff80000000)
    {
        *(.rodata*)
        . = ALIGN(4096);
    } > kernel

    .data : AT(ADDR(.data) - 0xffffffff80000000)
    {
        *(.data*)
        . = ALIGN(4096);
    } > kernel

    .bss : AT(ADDR(.bss) - 0xffffffff80000000)
    {
        bss_start = .;
        *(COMMON)
        *(.bss*)
        *(.lbss*)
        . = ALIGN(4096);
        bss_end = .;
    } > kernel

    kernel_end = .;
}
