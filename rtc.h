#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include "mmio.h"
#include "uart.h"

int rtc_probe(void);

void rtc_init(void);

uint32_t rtc_read_seconds(void);

#endif
