#ifndef __IOPORT__
#define __IOPORT__

#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#endif /* __IOPORT__ */
