#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

struct IDT_descriptor {
	uint16_t offset_1;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_attr;
	uint16_t offset_2;
	uint32_t offset_3;
	uint32_t zero;
};

struct IDT_ptr {
	uint16_t size;
	uintptr_t ptr;
} __attribute__((packed));

struct IRQ_frame {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rbp;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	uint64_t irq_num;
	uint64_t err_code;
};

void setup_idt(void);
void enable_interrupts(void);

/* irq handlers */
extern void vector0(void);
extern void vector1(void);
extern void vector2(void);
extern void vector3(void);
extern void vector4(void);
extern void vector5(void);
extern void vector6(void);
extern void vector7(void);
extern void vector8(void);
extern void vector10(void);
extern void vector11(void);
extern void vector12(void);
extern void vector13(void);
extern void vector14(void);
extern void vector16(void);
extern void vector17(void);
extern void vector18(void);
extern void vector19(void);
extern void vector32(void);
extern void vector39(void);

#endif
