#include "malloc.h"

static size_t	Ceil_to_pagesize_integer(size_t size) {
	size_t	computer_page_size = getpagesize();

	return (ceilling_unsigned(size, computer_page_size) * computer_page_size);
}

size_t	determine_page_size(t_zone *zone, size_t size_to_be_malloc) {
	size_t	page_size;

	if (zone->max_size == 0) {
		page_size = padded(size_to_be_malloc) + PAGE_OVERHEAD + MINIMUM_ALLOCATED_BLOCK_SIZE;
	}
	else if (RED_ZONE_SIZE != 0 && zone->max_size <= 256) {
		page_size = zone->max_size * 256;
	}
	else {
		page_size = zone->max_size * 128;
	}
	return Ceil_to_pagesize_integer(page_size);
}