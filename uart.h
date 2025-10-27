#ifndef UART_H
#define UART_H

#include "mmio.h"
#include <stdint.h>

void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
void uart_gets(char *buffer, int max_len);
void uart_puthex(uint64_t v);

#endif
