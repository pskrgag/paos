#ifndef __PIC_H__
#define __PIC_H__

#include <stdint.h>

#define PIC_MASTER_CMD		0x0020
#define PIC_MASTER_DATA		(PIC_MASTER_CMD + 1)

#define PIC_SLAVE_CMD		0x00A0
#define PIC_SLAVE_DATA		(PIC_SLAVE_CMD + 1)

void pic_eoi(uint8_t irq_num);

#endif /* __PIC__ */