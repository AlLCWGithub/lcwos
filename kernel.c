#include "uart.h"

int strcmp(const char *a, const char *b){
while(*a && *b && (*a == *b)){
++a;
++b;
}
return *(unsigned char *)a - *(unsigned char *)b;
}

void memorytest(void){
volatile uint32_t *start = (uint32_t *)0x40400000;
volatile uint32_t *end = (uint32_t *)0x40500000;
for(volatile uint32_t *p = start; p < end; ++p){
*p = (uint32_t)(p - start);
}
for(volatile uint32_t *p = start; p < end; ++p){
if(*p != (uint32_t)(p - start)){
uart_puts("Memory test failed! (incremental)\n");
return;
}
}
for(volatile uint32_t *p = start; p < end; ++p){
*p = 0xAAAAAAAA;
}
for(volatile uint32_t *p = start; p < end; ++p){
if(*p != 0xAAAAAAAA){
uart_puts("Memory test failed! (pattern A)\n");
return;
}
}
for(volatile uint32_t *p = start; p < end; ++p){
*p = 0x55555555;
}
for(volatile uint32_t *p = start; p < end; ++p){
if(*p != 0x55555555){
uart_puts("Memory test failed! (pattern B)\n");
return;
}
}
uart_puts("Memory test passed!\n");
for(volatile uint32_t *p = start; p < end; ++p){
*p = 0;
}
uart_puts("Memory reinitialised!\n");
}

void stacksaveloadtest(void){
volatile uint64_t a = 0x11111111UL;
volatile uint64_t b = 0x22222222UL;
volatile uint64_t c = 0x33333333UL;
volatile uint64_t teststack[3];
teststack[0] = a;
teststack[1] = b;
teststack[2] = c;
a = 0;
b = 0;
c = 0;
a = teststack[0];
b = teststack[1];
c = teststack[2];
uart_puthex(a);
uart_putc('\n');
uart_puthex(b);
uart_putc('\n');
uart_puthex(c);
uart_putc('\n');
if(a == 0x11111111UL && b == 0x22222222UL && c == 0x33333333UL){
uart_puts("Stack save and load test passed!\n");
}
else{
uart_puts("Stack save and load test failed!\n");
}
}

int stackframetest(int depth, volatile uint64_t pattern){
volatile uint64_t frame[2];
frame[0] = pattern;
frame[1] = (uint64_t)-pattern;
uart_puts("Frame values (before):\n");
uart_puthex(frame[0]);
uart_putc('\n');
uart_puthex(frame[1]);
uart_putc('\n');
if(depth > 0){
stackframetest(depth - 1, pattern + 1);
}
uart_puts("Frame values (after):\n");
uart_puthex(frame[0]);
uart_putc('\n');
uart_puthex(frame[1]);
uart_putc('\n');
if(frame[0] != pattern || frame[1] != (uint64_t)-pattern){
uart_puts("Stack frame corruption detected!\n");
return 1;
}
return 0;
}

void stacktest(int depth, volatile uint64_t pattern){
stacksaveloadtest();
if(stackframetest(depth, pattern) == 0){
uart_puts("Stack frame test passed!\n");
}
else{
uart_puts("Stack frame test failed!\n");
}
}

void svcexceptiontest(void){
__asm__ volatile("svc #0");
}

void memoryfaultexceptiontest(void){
volatile uint64_t *badaddr = (volatile uint64_t *)0xFFFFFFFFFFFFFFFF;
*badaddr = 0x11111111;
}

void handle_command(const char *cmd){
if(strcmp(cmd, "hello") == 0){
uart_puts("Hello!\n");
}
else if(strcmp(cmd, "reboot") == 0){
extern void _start(void);
_start();
}
else if(strcmp(cmd, "memorytest") == 0){
memorytest();
}
else if(strcmp(cmd, "stacktest") == 0){
stacktest(20, 0x11111111UL);
}
else if(strcmp(cmd, "exceptiontest") == 0){
svcexceptiontest();
memoryfaultexceptiontest();
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
