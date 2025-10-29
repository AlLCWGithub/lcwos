.section .text
.global _vector_table
.global _sync_el0
.global _sync_el1_spx
.global _unknown_sync
.global _default_handler
.global _irq_el1
.extern uart_puts
.extern uart_puthex
.align 11

_vector_table:
.align 7
b _sync_el0
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _sync_el1_spx
.align 7
b _irq_el1
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler
.align 7
b _default_handler

_sync_el0:
stp x29, x30, [sp, #-16]!
stp x0, x1, [sp, #-16]!
ldr x0, =msg_el0
bl uart_puts
mrs x0, FAR_EL1
bl uart_puthex
ldp x0, x1, [sp], #16
ldp x29, x30, [sp], #16
eret

_sync_el1_spx:
stp x29, x30, [sp, #-16]!
stp x0, x1, [sp, #-16]!
mrs x0, ESR_EL1
lsr x1, x0, #26
cmp x1, #0x15
b.eq _handle_svc
cmp x1, #0x24
b.eq _handle_data_abort
cmp x1, #0x25
b.eq _handle_data_abort
b _unknown_sync

_handle_svc:
ldr x0, =msg_svc
bl uart_puts
mrs x0, FAR_EL1
bl uart_puthex
ldr x0, =msg_n
bl uart_puts
ldp x0, x1, [sp], #16
ldp x29, x30, [sp], #16
eret

_handle_data_abort:
ldr x0, =msg_data_abort
bl uart_puts
mrs x0, FAR_EL1
bl uart_puthex
ldr x0, =msg_n
bl uart_puts
mrs x0, ELR_EL1
add x0, x0, #4
msr ELR_EL1, x0
ldp x0, x1, [sp], #16
ldp x29, x30, [sp], #16
eret

_unknown_sync:
ldr x0, =msg_unknown
bl uart_puts
ldp x0, x1, [sp], #16
ldp x29, x30, [sp], #16
eret

_irq_el1:
stp x29, x30, [sp, #-16]!
stp x0, x1, [sp, #-16]!


_default_handler:
ldr x0, =msg_default
bl uart_puts
b .

.section .rodata
msg_el0:
.asciz "Sync exception occured! (EL0)\n"

msg_el1:
.asciz "Sync exception occured! (EL1)\n"

msg_svc:
.asciz "Sync exception occured! (SVC exception)\n"

msg_data_abort:
.asciz "Sync exception occured! (Data abort)\n"

msg_unknown:
.asciz "Sync exception occured! (unknown)\n"

msg_default:
.asciz "I am not done with other handlers, you should not be here lol!\n"
msg_n:
.asciz "\n"
