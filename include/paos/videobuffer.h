#ifndef VIDEO_BUFFER_H
#define VIDEO_BUFFER_H

#define VIDEO_MEM_BASE		0xb8000

enum colors {
	Black,
	Blue,
	Green,
	Cyan,
	Red,
	Purple,
	Brown,
	Gray,
	Dark_blue,
	Light_blue,
	Light_green,
	Light_red,
	Light_purple,
	Yellow,
	White
};

void printk(const char *msg, ...);

#endif