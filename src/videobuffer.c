#include <stdarg.h>

#include <paos/videobuffer.h>
#include <paos/string.h>

static uint16_t *video_buffer = (uint16_t *) 0xb8000;

static int video_raw;
static int video_column;
enum colors current_color = Light_blue;

#define VIDEO_WIDTH			80
#define VIDEO_HEIGHT			25

#define __INDEX__			video_raw * VIDEO_WIDTH + video_column
#define __CHAR__(chr, color)		(chr | color << 8)

static void putchar(char chr)
{
	if (chr == '\n') {
		video_raw++;
		video_column = 0;
		return;
	}

	video_buffer[__INDEX__] = __CHAR__(chr, current_color);

	if (++video_column == VIDEO_WIDTH) {
		video_column = 0;
		if (++video_raw == VIDEO_HEIGHT)
			video_raw = 0;
	}
}

static void putstring(const char *str)
{
	int i;

	for (int i = 0; str[i]; ++i)
		putchar(str[i]);
}

static int hex_to_string(char *buffer, uint64_t digits)
{
	char digits_buffer[25];
	char digits_map[16] = "0123456789abcdef";
	int size = 0, position = 0;

	do {
		digits_buffer[size++] = digits_map[digits % 16];
		digits /= 16;
	} while (digits != 0);

	for (int i = size - 1; i >= 0; i--) {
		buffer[position++] = digits_buffer[i];
	}

	return size + 1;
}

void printk(const char *fmt, ...)
{
	int i;
	va_list list;
	char buffer[1024] = {0};

	va_start(list, fmt);

	while (*fmt) {
		if (*fmt != '%') {
			putchar(*fmt);
			goto while_end;
		}

		switch (*(++fmt)) {
		case 's':
			putstring(va_arg(list, char *));
			break;
		case 'c':
			putchar(va_arg(list, int));
			break;
		case 'd':
			itoa(va_arg(list, int32_t), buffer);
			putstring(buffer);
			break;
		case 'f':
			ftoa(va_arg(list, double), buffer, 6);
			putstring(buffer);
			break;
		case 'x':
			hex_to_string(buffer, va_arg(list, uint64_t));
			putstring(buffer);
			break;
		default:
			--fmt;
			putchar(*fmt);
			break;
		}

		memset(buffer, 0, sizeof(buffer));
while_end:
		++fmt;
	}
}
