#ifndef _KERNEL_IO_H
#define _KERNEL_IO_H
#include <stdint.h>

void io_outb(uint16_t port, uint8_t data);
uint8_t io_inb(uint16_t port);

#endif
