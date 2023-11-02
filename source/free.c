#include "malloc.h"

void	free(void *ptr) {
	dprintf(1, "my free");
	free(ptr);
}