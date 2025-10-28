#include "uart.h"

int strcmp(const char *a, const char *b){
while(*a && *b && (*a == *b)){
++a;
++b;
}
return *(unsigned char *)a - *(unsigned char *)b;
}

void memorytest(void){
volatile uint32_t *start = (uint32_t *)0x40100000;         volatile uint32_t *end = (uint32_t *)0x40200000;           for(volatile uint32_t *p = start; p < end; ++p){           *p = (uint32_t)(p - start);
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
uart_puthex(b);
uart_puthex(c);
if(a == 0x11111111UL && b == 0x22222222UL && c == 0x33333333UL){
uart_puts("Stack save and load test passed!\n");
}
else{
uart_puts("Stack save and load test failed!\n");
}
}

void stackframetest(int depth, volatile uint64_t pattern){
volatile uint64_t frame[2];
frame[0] = pattern;
frame[1] = (uint64_t)-pattern;
uart_puts("Frame values:\n");
uart_puthex(frame[0]);
uart_putc('\n');
uart_puthex(frame[1]);
uart_putc('\n');
if(depth > 0){
stackframetest(depth - 1, pattern + 1);
}
uart_puts("Frame values: \n");
uart_puthex(frame[0]);
uart_putc('\n');
uart_puthex(frame[1]);
if(frame[0] != pattern || frame[1] != (uint64_t)-pattern){
uart_puts("Stack frame corruption detected!\n");
}
}

void stacktest(int depth, volatile uint64_t pattern){
stacksaveloadtest();
stackframetest(depth, pattern);
}

void exceptiontest(void){
__asm__ volatile("svc #0");
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
stacktest(2, 0x11111111UL);
}
else if(strcmp(cmd, "exceptiontest") == 0){
exceptiontest();
}
else{
uart_puts("Unknown command!\n");
}
}

void kernel_main(void){
char input[64];
uart_puts("Welcome to LCWos!\n");
uart_puts("Input something!\n");
volatile uint64_t stack[1];
stack[0] = 0x11111111UL;
uart_puthex(stack[0]);
uart_putc('\n');
for(;;){
uart_puts("> ");
uart_gets(input, sizeof(input));
handle_command(input);
}
}
