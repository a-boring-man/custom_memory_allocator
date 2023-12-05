#include "malloc.h"
#include <sys/mman.h>

void	*create_page(t_zone *zone, size_t size) {
	void	*new_page = NULL;

	if (zone->max_size == 0) {
		new_page = mmap(0, size + MINIMUM_PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
	}
	else if (RED_ZONE_SIZE != 0 && zone->max_size <= 256) {
		new_page = mmap(0, zone->max_size * 256, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
	}
	else {
		new_page = mmap(0, zone->max_size * 128, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
	}
	return new_page;
}