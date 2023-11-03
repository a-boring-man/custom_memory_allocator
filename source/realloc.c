#include "malloc.h"

void	*realloc(void *ptr, size_t size) {
	dprintf(1, "my realloc %ld, %p", size, ptr);
	return(NULL);
}