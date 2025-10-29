.section .text
.global _start
.align 7

_start:
ldr x0, =_stack_top
mov sp, x0
ldr x0, =_vector_table
msr VBAR_EL1, x0
isb
bl kernel_main

hang:
wfe
b hang
