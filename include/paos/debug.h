#ifndef __DEBUG__
#define __DEBUG__

#include <stdint.h>

#define assert(e)				\
do {						\
	if (!(e))				\
		__error(__FILE__, __LINE__);	\
} while (0)


void __attribute__((noreturn)) __error(const char *str, uint64_t line);

#endif /* __DEBUG__ */