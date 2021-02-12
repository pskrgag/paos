#include <paos/pic.h>
#include <paos/ioport.h>

#define PIC_EOI		0x20

void pic_eoi(uint8_t irq_num)
{
	if(irq_num >= 8)
		outb(PIC_SLAVE_CMD, PIC_EOI);

	outb(PIC_MASTER_CMD, PIC_EOI);
}
