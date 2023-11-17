#include "malloc.h"
#include <sys/mman.h>

void	*create_page(t_zone *zone, size_t size) {
	void	*new_page = NULL;
	size_t	page_size;
	size_t	computer_page_size = getpagesize();

	if (zone->max_size == 0) {
		new_page = mmap(0, size + 16, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
		page_size = ceilling_unsigned((double)(size + 16) / (double)computer_page_size) * computer_page_size; // compute the alocated page TRUE size
	}
	else {
		new_page = mmap(0, zone->max_size * 128, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
		page_size = ceilling_unsigned((double)(zone->max_size * 128) / (double)computer_page_size) * computer_page_size; // compute the allocated page TRUE size
	}
	return new_page;
}