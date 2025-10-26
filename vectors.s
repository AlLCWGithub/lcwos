.section .text
.global _vector_table
.align 11

_vector_table:
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry
b exception_entry

exception_entry:
ldr x0, =_stack_top
mov sp, x0
ldr x0, =msg
bl uart_puts

mrs x1, elr_el1
add x1, x1, #4
msr elr_el1, x1
eret
.section .rodata
msg:
.asciz "Exception occured!\n"
