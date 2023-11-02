#include "malloc.h"

void	*my_realloc(void *ptr, size_t size) {
	dprintf(1, "my realloc");
	realloc(ptr, size);
}