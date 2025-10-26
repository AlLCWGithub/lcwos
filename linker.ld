OUTPUT_FORMAT("elf64-littleaarch64");
ENTRY(_start);

MEMORY{
RAM_TEXT (rx) : ORIGIN = 0x40000000, LENGTH = 1M
RAM_DATA (rw) : ORIGIN = 0x40100000, LENGTH = 1M
RAM_STACK (rw) : ORIGIN = 0x40400000, LENGTH = 1M
RAM_HEAP (rw) : ORIGIN = 0x40500000, LENGTH = 1M
RAM_EXCEPTION (rw) : ORIGIN = 0x40600000, LENGTH = 4K
}

SECTIONS{
. = 0x40000000;
.text : { *(.text*) } > RAM_TEXT
.rodata : { *(.rodata*) } > RAM_TEXT
.data : { *(.data*) } > RAM_DATA
.bss : { *(.bss*) } > RAM_DATA
.stack : { . = ORIGIN(RAM_STACK) + LENGTH(RAM_STACK); . = ALIGN(16); _stack_top = .; } > RAM_STACK
.heap : { *(.heap*) } > RAM_HEAP
.exception : { . = ORIGIN(RAM_EXCEPTION) + LENGTH(RAM_EXCEPTION); . = ALIGN(16); _exception_stack_top = .; } > RAM_EXCEPTION
}
