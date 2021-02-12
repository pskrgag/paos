#include <stdint.h>
#include <stddef.h>

#include <paos/string.h>
#include <paos/math.h>

uint64_t strlen(const char *str)
{
	int i;
	uint64_t size = 0;

	for (i = 0; str[i]; ++i)
		++size;

	return size;
}

void *memcpy(void *dst, const void *src, uint64_t size)
{
	int i;

	for (i = 0; i < size; ++i)
		((char *) dst)[i] = ((char *) src)[i];

	return dst;
}

void *memset(void *dst, char cr, uint64_t size)
{
	int i;

	for (i = 0; i < size; ++i)
		((char *) dst)[i] = cr;

	return dst;
}

int memcmp(const void *dst, const void *src, uint64_t size)
{
	int i;

	for (i = 0; i < size; ++i) {
		if (((char *) dst)[i] > ((char *) src)[i])
			return 1;
		else if (((char *) dst)[i] < ((char *) src)[i])
			return -1;
	}

	return 0;
}

void strcpy(char *dst, const char *src)
{
	int i;

	for (i = 0; src[i]; ++i)
		dst[i] = src[i];
}


static void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;

	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

static int __itoa(int x, char str[], int d)
{
	int i = 0;

	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	while (i < d)
		str[i++] = '0';

	reverse(str, i);
	str[i] = '\0';

	return i;
}

void ftoa(float n, char *out, int af)
{
	int ipart = (int) n;
	float fpart = n - (float) ipart;
	int i = __itoa(ipart, out, 0);

	if (af != 0) {
		out[i] = '.';
		fpart = fpart * pow(10, af);
		__itoa((int) fpart, out + i + 1, af);
	}
}

void itoa(int num, char *out)
{
	__itoa(num, out, 0);
}
