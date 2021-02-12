#ifndef STRING_H
#define STRING_H

#include <stdint.h>

uint64_t strlen(const char *str);

void *memcpy(void *dst, const void *src, uint64_t size);
void *memset(void *dst, char cr, uint64_t size);
int memcmp(const void *dst, const void *src, uint64_t size);

void itoa(int num, char *out);
void ftoa(float n, char *out, int af);

void strcpy(char *dst, const char *src);
#endif