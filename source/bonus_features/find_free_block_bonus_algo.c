#include "malloc.h"

void	*best_fit(size_t size_to_be_alloc, t_zone *zone) { // size if the raw size
	t_list	*list_head = zone->free;
	size_t	left_over = MAX_SIZET;
	t_list	*best_block = NULL;
	size_t	true_size = padded(size_to_be_alloc) + MINIMUM_ALLOCATED_BLOCK_SIZE;

	// whyle free list is not empty and free block size <= size_to_be_allocated continue
	while (list_head != NULL \
		&& list_head->next != zone->free \
		&& left_over != 0)
	{
		if (*((size_t *)list_head - 1) >= true_size && left_over > (*((size_t *)list_head -1) - true_size)) {
			best_block = list_head;
			left_over = *((size_t *)list_head -1) - true_size;
		}
		list_head = list_head->next;
	}
	if (list_head == NULL || (*((size_t *)list_head - 1) < true_size && best_block == NULL)) { // no block was found need to create a new page
		t_memory_pointer	new_page;

		new_page.as_void = create_page(zone, size_to_be_alloc); 
		format_new_page(new_page.as_void, determine_page_size(zone, size_to_be_alloc));
		add_block_to_t_list((t_list *)(new_page.as_sizeT + 1), (t_list **)(&(zone->page)));
		add_block_to_t_list((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), (t_list **)(&(zone->free)));
		return (mark_block_as_allocated((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), padded(size_to_be_alloc), zone));
	}
	else if (*((size_t *)list_head - 1) >= true_size && left_over > (*((size_t *)list_head -1) - true_size)) { // lats member is the best
		return (mark_block_as_allocated(list_head, padded(size_to_be_alloc), zone));
	}
	else { // a best block was found in the list
		return (mark_block_as_allocated(best_block, padded(size_to_be_alloc), zone));
	}
}

void	*next_fit(size_t size_to_be_alloc, t_zone *zone) {
	if (zone->next == NULL || (*((size_t *)zone->next -1) < padded(size_to_be_alloc))) { // if free list was empty
		t_memory_pointer	new_page;

		new_page.as_void = create_page(zone, size_to_be_alloc); 
		format_new_page(new_page.as_void, determine_page_size(zone, size_to_be_alloc));
		add_block_to_t_list((t_list *)(new_page.as_sizeT + 1), (t_list **)(&(zone->page)));
		add_block_to_t_list((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), (t_list **)(&(zone->free)));
		return (mark_block_as_allocated((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), padded(size_to_be_alloc), zone));
	}
	else {
		return (mark_block_as_allocated(zone->next, padded(size_to_be_alloc), zone));
	}
}
