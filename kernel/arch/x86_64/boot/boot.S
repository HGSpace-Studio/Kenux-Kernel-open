%define MULTIBOOT_HEADER_MAGIC 0x1BADB002
%define MULTIBOOT_HEADER_FLAGS 0x0

section .text
global _start
extern kernel_main
_start:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

    cli
    mov rsp, stack_top

    call kernel_main

hlt_loop:
    hlt
    jmp hlt_loop

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
