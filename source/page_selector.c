#include "malloc.h"

t_zone	*choose_the_right_page(size_t size) {
	if (size <= 8)
		return &grimoire[0];
	else if (size <= 16)
		return &grimoire[1];
	else if (size <= 32)
		return &grimoire[2];
	else if (size <= 64)
		return &grimoire[3];
	else if (size <= 96)
		return &grimoire[4];
	else if (size <= 128)
		return &grimoire[5];
	else if (size <= 192)
		return &grimoire[6];
	else if (size <= 256)
		return &grimoire[7];
	else if (size <= 512)
		return &grimoire[8];
	else if (size <= 1024)
		return &grimoire[9];
	else if (size <= 2048)
		return &grimoire[10];
	else if (size <= 4096)
		return &grimoire[11];
	else if (size <= 8192)
		return &grimoire[12];
	else
		return &grimoire[13];
}