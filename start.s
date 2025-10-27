.section .text
.global _start
.align 7

_start:
ldr x0, =_stack_top
mov sp, x0
bl kernel_main

hang:
wfe
b hang
