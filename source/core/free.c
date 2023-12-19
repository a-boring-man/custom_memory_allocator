#include "malloc.h"

static void check_for_unmap_page() {
	for (int i = 0; i < 11; i++){
		int is_page_empty = 0;
		t_memory_pointer	working_pointer;
		working_pointer.as_void = grimoire[i].page;
		if (working_pointer.as_void == NULL) {
			continue;
		}
		size_t	page_size = *working_pointer.as_sizeT; // register the page size
		working_pointer.as_char += PAGE_START_OVERHEAD; // move the pointer the begginning of the free list
		if (!(*working_pointer.as_sizeT & 1) && *working_pointer.as_sizeT + PAGE_OVERHEAD == page_size) { // if the block is free and take up all the page
			remove_block_from_t_list((t_list *)(working_pointer.as_char - (PAGE_START_OVERHEAD - sizeof(size_t))), &(grimoire[i].page)); // remove the page from the page list
			
		}
	}
}

void	free(void *ptr) {
	size_t	block_size;
	size_t	data_size;
	static int free_page_counter = 256;
	
	if (ptr == NULL) {
		return;
	}

	block_size = *(size_t *)((char *)ptr - (sizeof(size_t) + RED_ZONE_SIZE)) & -2;
	data_size = block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;
	ft_printf("wanting to free the block at : -%p- of size : -%u- and of true size : -%u-\n", ptr, block_size, data_size);
	t_zone	*zone = choose_the_right_page(data_size);
	
	
	mark_block_as_free(ptr, zone);
	coalescing(ptr, zone);
	check_for_unmap_page();
	
	(void)ptr;
	int pff = write(1, "coucou\n", 7);
	(void)pff;
}