CROSS = aarch64-linux-gnu-
CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld
CFLAGS = -nostdlib -nostartfiles -ffreestanding -fno-stack-protector -O2 -Wall -Wextra -mcpu=cortex-a53
LDFLAGS = -T linker.ld
OBJS = start.o kernel.o uart.o vectors.o rtc.o

all: kernel.elf

start.o: start.s
	$(AS) start.s -o start.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

uart.o: uart.c uart.h
	$(CC) $(CFLAGS) -c uart.c -o uart.o

vectors.o: vectors.s
	$(AS) vectors.s -o vectors.o

rtc.o: rtc.c
	$(CC) $(CFLAGS) -c rtc.c -o rtc.o

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.elf

clean:
	rm -f *.o kernel.elf

.PHONY: all clean
