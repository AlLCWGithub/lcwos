#include <stdint.h>
#include "mmio.h"
#include "uart.h"

#define PL031_BASE 0x9010000UL
#define RTCDR 0x000
#define RTCMR 0x004
#define RTCLR 0x008
#define RTCCR 0x00C
#define RTCIMSC 0x010
#define RTCRIS 0x014
#define RTCMIS 0x018
#define RTCICR 0x01C
#define RTC_PERIPH_ID0 0xFE0
#define PL031_ID0 0x31

static inline uint32_t rtc_read(uint32_t offset){
return mmio_read32((volatile uint32_t *)(PL031_BASE + offset));
}

static inline void rtc_write(uint32_t offset, uint32_t value){
mmio_write32((volatile uint32_t *)(PL031_BASE + offset), value);
}

int rtc_probe(void){
uint8_t id0 = (uint8_t)rtc_read(RTC_PERIPH_ID0);
return (id0 == PL031_ID0);
}

void rtc_init(void){
if(rtc_probe()){
uart_puts("PL031 detected at ");
uart_puthex(PL031_BASE);
uart_puts("!\n");
rtc_write(RTCCR, 1);
}
else{
uart_puts("PL031 not detected!\n");
}
}

uint32_t rtc_read_seconds(void){
return rtc_read(RTCDR);
}
