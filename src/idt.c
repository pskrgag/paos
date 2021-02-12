#include <paos/idt.h>
#include <paos/kernel.h>
#include <paos/videobuffer.h>
#include <paos/pic.h>

struct IDT_entry {
	void *handler;
};

static struct IDT_descriptor IDT_table[256] = {
	[0 ... 255] = {
		.selector = 0x8,		/* code segment */
		.type_attr = 0x8e,
	}
};

const static struct IDT_ptr IDT_ptr = {
	.size = sizeof(IDT_table) - 1,
	.ptr = (uintptr_t) IDT_table,
};

static struct IDT_entry handlers[256] = {
	[0] = vector0,
	[1] = vector1,
	[2] = vector2,
	[3] = vector3,
	[4] = vector4,
	[5] = vector5,
	[6] = vector6,
	[7] = vector7,
	[8] = vector8,
	[10] = vector10,
	[11] = vector11,
	[12] = vector12,
	[13] = vector13,
	[14] = vector14,
	[16] = vector16,
	[17] = vector17,
	[18] = vector18,
	[19] = vector19,
	[32] = vector32,
	[39] = vector39,

};

#define __IS_ENTRY_EMPTY(entry)		(entry.handler == NULL)


static inline void load_idt(void)
{
	__asm__ volatile("lidt %0" :: "m" (IDT_ptr));
}

static inline void set_idt_entry(struct IDT_entry *entry, char index)
{
	const uintptr_t addr = (uintptr_t) entry->handler;

	IDT_table[index].offset_1 = (uint16_t) addr;
	IDT_table[index].offset_2 = (uint16_t) (addr >> 16);
	IDT_table[index].offset_3 = (uint32_t) (addr >> 32);
}

void setup_idt(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(handlers); ++i) {
		if (!__IS_ENTRY_EMPTY(handlers[i]))
			set_idt_entry(handlers + i, i);
	}

	load_idt();
}

void irq_handler(struct IRQ_frame *frame)
{
	switch (frame->irq_num) {
	case 0:
		break;
	case 32:
		pic_eoi(32);
		break;
	case 39:
		pic_eoi(39);
	default:
		break;
	}
}

void enable_interrupts(void)
{
	__asm__ volatile("sti\n");
}
