#include "malloc.h"

size_t	Ceil_to_pagesize_integer(size_t size) {
	size_t	computer_page_size = getpagesize();

	return (ceilling_unsigned((double)size / (double)computer_page_size) * computer_page_size);
}

size_t	determine_page_size(t_zone *zone, size_t size_to_be_malloc) {
	size_t	page_size;

	if (zone->max_size == 0) {
		page_size = size_to_be_malloc + 3 * sizeof(size_t) + 2 * RED_ZONE_SIZE;
	}
	else if (RED_ZONE_SIZE > 4 && zone->max_size == 64) {
		page_size = zone->max_size * 256;
	}
	else if (RED_ZONE_SIZE > 8 && zone->max_size == 128) {
		page_size = zone->max_size * 256;
	}
	else {
		page_size = zone->max_size * 128;
	}
	return Ceil_to_pagesize_integer(page_size);
}