.section .text
.global _start
.align 7

_start:
ldr x0, =_stack_top
mov sp, x0
isb
ldr x0, =_vector_table
msr VBAR_EL1, x0
isb
mrs x19, cntvct_el0
dsb sy
msr daifclr, #2
isb
bl _gic_init
dsb sy
isb
bl kernel_main

_gic_init:
ldr x0, =0x08000000
mov w1, #1
str w1, [x0]
ldr x0, =0x08000100
mov w1, #(1 << 27)
str w1, [x0]
ldr x0, =0x08010004
mov w1, #0xFF
str w1, [x0]
ldr x0, =0x08010000
mov w1, #1
str w1, [x0]
dsb sy
isb
ret

_hang:
wfe
b _hang
