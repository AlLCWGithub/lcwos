.section .text
.balign 2048
.global _vector_table
.global _default_handler
.global _sync_el1_spx
.extern uart_puts

_vector_table:
b _sync_el0
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _sync_el1_spx
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _sync_lower_aarch64
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler
.balign 0x80
b _default_handler

_sync_el0:
stp x19, x20, [sp, #-16]!
ldr x0, =sync_el1_spx_msg
bl uart_puts
mrs x1, elr_el1
add x1, x1, #4
msr elr_el1, x1
isb
ldp x19, x20, [sp], #16
eret

_sync_el1_spx:
stp x19, x20, [sp, #-16]!
ldr x0, =sync_el1_spx_msg
bl uart_puts
mrs x1, elr_el1
add x1, x1, #4
msr elr_el1, x1
isb
ldp x19, x20, [sp], #16
eret

_sync_lower_aarch64:
stp x19, x20, [sp, #-16]!
ldr x0, =sync_el1_spx_msg
bl uart_puts
mrs x1, elr_el1
add x1, x1, #4
msr elr_el1, x1
isb
ldp x19, x20, [sp], #16
eret

_default_handler:
b .

.section .rodata
sync_el1_spx_msg:
.asciz "Sync exception occured!\n"
