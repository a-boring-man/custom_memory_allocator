#include "malloc.h"
#include <sys/mman.h>

void	*create_page(t_zone *zone, size_t size) {
	void	*new_page = NULL;
	
	if (zone->max_size == 0)
		new_page = mmap(0, size + 16, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
	else
		new_page = mmap(0, zone->max_size * 128, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
	return glue_page_together(zone, new_page, size);
}