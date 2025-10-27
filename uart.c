#include "mmio.h"
#include <stdint.h>

#define UART0_BASE 0x09000000UL
#define UARTDR (volatile uint32_t *)(UART0_BASE + 0x00)
#define UARTFR (volatile uint32_t *)(UART0_BASE + 0x18)

void uart_putc(char c){
while(mmio_read32(UARTFR) & (1 << 5)){
}
mmio_write32(UARTDR, (uint32_t)c);
}

void uart_puts(const char *s){
for(const char *p = s; *p; ++p){
uart_putc(*p);
}
}

char uart_getc(void){
while(mmio_read32(UARTFR) & (1 << 4)){
}
return (char)(mmio_read32(UARTDR) & 0xFF);
}

void uart_gets(char *buffer, int max_len){
int i = 0;
for(;;){
char c = uart_getc();
if(c == '\r' || c == '\n'){
uart_putc('\n');
break;
}
if(c == '\b' || c == 0x7F){
if(i > 0){
--i;
uart_putc('\b');
uart_putc(' ');
uart_putc('\b');
}
continue;
}
if(i < max_len - 1){
buffer[i++] = c;
uart_putc(c);
}
}
buffer[i] = '\0';
}

void uart_puthex(uint64_t x){
char buffer[17];
for(int i = 0; i < 16; ++i){
int shift = (15 - i) * 4;
int nib = (x >> shift) & 0xF;
buffer[i] = (nib < 10) ? ('0' + nib) : ('a' + (nib - 10));
}
buffer[16] = '\0';
uart_puts("0x");
uart_puts(buffer);
}
