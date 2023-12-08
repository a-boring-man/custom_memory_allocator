#include "malloc.h"

void	*first_fit(size_t size_to_be_alloc, t_zone *zone) {
	t_list	*list_head = zone->free;

	// whyle free list is not empty and free block size <= size_to_be_allocated continue
	while (list_head != NULL \
		&& list_head->next != zone->free \
		&& *((size_t *)(list_head) - 1) < padded(size_to_be_alloc) + MINIMUM_ALLOCATED_BLOCK_SIZE)
	{
		list_head = list_head->next;
	}
	if (list_head == NULL || (list_head->next == zone->free && *((size_t *)(list_head) - 1) < padded(size_to_be_alloc) + MINIMUM_ALLOCATED_BLOCK_SIZE)) { // no block was found need to create a new page
		t_memory_pointer	new_page;

		new_page.as_void = create_page(zone, size_to_be_alloc); 
		format_new_page(new_page.as_void, determine_page_size(zone, size_to_be_alloc));
		add_block_to_t_list((t_list *)(new_page.as_sizeT + 1), (t_list **)(&(zone->page)));
		add_block_to_t_list((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), (t_list **)(&(zone->free)));
		return (mark_block_as_allocated((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), padded(size_to_be_alloc), zone));
	}
	else { // if a block was found
		return (mark_block_as_allocated(list_head, padded(size_to_be_alloc), zone));
	}
}