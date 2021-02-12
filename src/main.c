#include <stdint.h>
#include <stddef.h>

#include <paos/videobuffer.h>
#include <paos/idt.h>
#include <paos/mm.h>

#include <paos/debug.h>

void main(void)
{
	setup_idt();
	enable_interrupts();
	init_memory();
	printk("Starting the kernel... 0x%x\n", 10);
	printk("Loaded IDT...\n");
}