#include "malloc.h"

static size_t check_if_only_contain_free_block(void *first_free_block) { // the addres is the begginning of the size of the freeblock
	t_memory_pointer	working_pointer;
	
	working_pointer.as_void = first_free_block;
	while (!(*working_pointer.as_sizeT & 1)) { // stop if a block is allocated
		working_pointer.as_char += *working_pointer.as_sizeT; // jump to the next block
	}
	return (*working_pointer.as_sizeT == 1); // return true if we stop because we reach the marker of end of page
}

static int check_if_page_is_empty(void *page) {
	t_memory_pointer	working_pointer;
	size_t	page_size;
	size_t	has_only_free_block = 0;

	working_pointer.as_void = page;
	working_pointer.as_sizeT -= 1; // move the pointer to the beginning of the page
	page_size = *working_pointer.as_sizeT; // store page size
	working_pointer.as_char += PAGE_START_OVERHEAD;
	has_only_free_block = check_if_only_contain_free_block(working_pointer.as_void);
	return (!(*working_pointer.as_sizeT & 1) && (page_size == (*working_pointer.as_sizeT & -2) + PAGE_OVERHEAD || has_only_free_block)); // return true if the first block is not malloc AND either the hol page is a free block or composed of free block only
}

static void check_for_unmap_page(t_zone *zone) {
	for (int i = 0; i < 11; i++){
		if (grimoire[i].page == NULL) {
			continue;
		}
		remove_page_if(&(grimoire[i].page), check_if_page_is_empty, zone);
	}
}

void	free(void *ptr) {
	size_t	block_size;
	size_t	data_size;
	static int free_page_counter = FREE_DELAY; // the number of free before a page cleanup
	
	if (ptr == NULL) {
		return;
	}

	block_size = *(size_t *)((char *)ptr - (sizeof(size_t) + RED_ZONE_SIZE)) & -2;
	if (!block_size) {
		return;
	}
	data_size = block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;
	ft_printf("wanting to free the block at : -%p- of size : -%u- and of true size : -%u-\n", ptr, block_size, data_size);
	debug_hexa((void *)((size_t *)ptr - 4 - RED_ZONE_SIZE / sizeof(size_t)), (block_size / sizeof(size_t)) + 2 + RED_ZONE_SIZE);
	t_zone	*zone = choose_the_right_page(data_size);
	
	
	mark_block_as_free(ptr, zone);
	printf_t_list(zone->free);
	coalescing(ptr, zone);
	if (free_page_counter-- == 0) { // free all empty page once in a while
		check_for_unmap_page(zone);
		free_page_counter = FREE_DELAY;
	}
	
	ft_printf("free_page_counter ||||||||||||||||||||||: -%d-\n", free_page_counter);
}