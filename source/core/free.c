#include "malloc.h"

void	free(void *ptr) {
	//dprintf(1, "my free %p", ptr);
	(void)ptr;
	int pff = write(1, "coucou\n", 7);
	(void)pff;
}