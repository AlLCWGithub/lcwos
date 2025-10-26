#include "uart.h"

int strcmp(const char *a, const char *b){
while(*a && *b && (*a == *b)){
++a;
++b;
}
return *(unsigned char *)a - *(unsigned char *)b;
}

void handle_command(const char *cmd){
if(strcmp(cmd, "hello") == 0){
uart_puts("Hello!\n");
}
else if(strcmp(cmd, "reboot") == 0){
extern void _start(void);
_start();
}
else if(strcmp(cmd, "memtest") == 0){
volatile uint32_t *addr = (uint32_t *)0x40100000;
for(int i = 0; i < 16; ++i){
addr[i] = i;
}
for(int i = 0; i < 16; ++i){
if(addr[i] != (uint32_t)i){
uart_puts("Memory test failed!\n");
return;
}
}
uart_puts("Memory test passed!\n");
}
else if(strcmp(cmd, "exceptiontest") == 0){
volatile int *ptr = (volatile int *)0xFFFFFFFF; // outside mapped RAM
*ptr = 123;
}
else{
uart_puts("Unknown command!\n");
}
}

void kernel_main(void){
char input[64];
uart_puts("Welcome to LCWos!\n");
uart_puts("Input something!\n");
for(;;){
uart_puts("> ");
uart_gets(input, sizeof(input));
handle_command(input);
}
}
