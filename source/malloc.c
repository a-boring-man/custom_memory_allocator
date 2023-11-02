#include "malloc.h"

void	*my_malloc(size_t size) {
	dprintf(1, "my_malloc");
	return (malloc(size));
}