#include "malloc.h"

static int check_if_page_is_empty(void *page) {
	t_memory_pointer	working_pointer;
	size_t	page_size;

	working_pointer.as_void = page;
	working_pointer.as_sizeT -= 1; // move the pointer to the beginning of the page
	page_size = *working_pointer.as_sizeT; // store page size
	working_pointer.as_char += PAGE_START_OVERHEAD;
	return (page_size == *working_pointer.as_sizeT + PAGE_OVERHEAD);
}

static void check_for_unmap_page() {
	for (int i = 0; i < 11; i++){
		if (grimoire[i].page == NULL) {
			continue;
		}
		remove_page_if(&(grimoire[i].page), check_if_page_is_empty);
	}
}

void	free(void *ptr) {
	size_t	block_size;
	size_t	data_size;
	static int free_page_counter = 256; // the number of free before a page cleanup
	
	if (ptr == NULL) {
		return;
	}

	block_size = *(size_t *)((char *)ptr - (sizeof(size_t) + RED_ZONE_SIZE)) & -2;
	data_size = block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;
	ft_printf("wanting to free the block at : -%p- of size : -%u- and of true size : -%u-\n", ptr, block_size, data_size);
	t_zone	*zone = choose_the_right_page(data_size);
	
	
	mark_block_as_free(ptr, zone);
	coalescing(ptr, zone);
	if (free_page_counter-- == 0) { // free all empty page once in a while
		check_for_unmap_page();
		free_page_counter = 256;
	}
	
	//ft_printf("free_page_counter : -%d-\n", free_page_counter);
}