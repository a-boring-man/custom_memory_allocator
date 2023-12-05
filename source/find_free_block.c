#include "malloc.h"

/**
 * @brief write meta data next to the data to be malloced
 * 
 * @param block the very begginning of the free block to be malloced
 * @param block_size the size of the malloced content
 */
void	format_allocated_block(void	*block, size_t malloc_size) {// a check
	*((size_t *)block) = malloc_size + 1;
}

void	*best_fit(size_t size_to_be_alloc, t_zone *zone) {
	(void)size_to_be_alloc;
	(void)zone;
	return NULL;
}

void	*next_fit(size_t size_to_be_alloc, t_zone *zone) {
	(void)size_to_be_alloc;
	(void)zone;
	return NULL;
}

void	*first_fit(size_t size_to_be_alloc, t_zone *zone) {
	t_list	*list_head = zone->free;

	if (list_head == NULL) { // empty free list
		t_memory_pointer	new_page;
		new_page.as_void = create_page(zone, size_to_be_alloc); 
		format_new_page(new_page.as_void, determine_page_size(zone, size_to_be_alloc));
		add_block_to_t_list((t_list *)(new_page.as_sizeT + 1), (t_list **)(&(zone->page)));
		add_block_to_t_list((t_list *)(new_page.as_char + PAGE_OVERHEAD + sizeof(size_t)), (t_list **)(&(zone->free)));
	}
	return NULL;
}