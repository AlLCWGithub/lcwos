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
ldr sp =_exception_stack_top
mov x1, 

.section .rodata
msg:
.asciz "Exception occured!\n"
