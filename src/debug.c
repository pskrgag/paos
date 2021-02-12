#include <paos/debug.h>
#include <paos/videobuffer.h>

void __attribute__((noreturn)) __error(const char *str, uint64_t line)
{
	printk("Assertation failed at %s %d\n", str, line);

	while(1);
}