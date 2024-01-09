#include "malloc.h"

void	*first_fit(size_t size_to_be_alloc, t_zone *zone) {
	t_list	*list_head = zone->free;

	//ft_printf("entering first fit\n");
	// whyle free list is not empty and free block size <= size_to_be_allocated continue
	ft_printf("ICI ! list_head is %p\n", list_head);
	//show_alloc_mem();
	//if (list_head != NULL)
		//debug_hexa((void *)((size_t *)list_head), (*((size_t *)list_head - 1) / sizeof(size_t)));
	while (list_head != NULL \
		&& list_head->next != zone->free \
		&& *((size_t *)(list_head) - 1) < padded(size_to_be_alloc) + MINIMUM_ALLOCATED_BLOCK_SIZE)
	{
		ft_printf("in thw while loop\n");
	//ft_printf("has not segfault before checking the size, list_head_next == : -%p- and zone->free = : -%p-\n", list_head->next, zone->free);
		list_head = list_head->next;
	//ft_printf("has not segfault after jumping to the next element list_head : -%p-\n", list_head);
	}
	if (list_head == NULL || (list_head->next == zone->free && *((size_t *)(list_head) - 1) < padded(size_to_be_alloc) + MINIMUM_ALLOCATED_BLOCK_SIZE)) { // no block was found need to create a new page
		t_memory_pointer	new_page;
	
	//ft_printf("must allocate a new page\n");
		new_page.as_void = create_page(zone, size_to_be_alloc); 
		format_new_page(new_page.as_void, determine_page_size(zone, size_to_be_alloc));
	//ft_printf("new page was created and formated\n");
	//debug_hexa(new_page.as_void, 40);
		add_block_to_t_list((t_list *)(new_page.as_sizeT + 1), &(zone->page));
	//debug_hexa(new_page.as_void, 40);
		add_block_to_t_list((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), &(zone->free));
		//printf_t_list(zone->page);
		//printf_t_list(zone->free);
	//ft_printf("the page was added to page t_list and the free block inside to the free t_list zone->free : -%p- and zone->page : -%p-\n", zone->free, zone->page);
		return (mark_block_as_allocated((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), padded(size_to_be_alloc), zone));
	}
	else { // if a block was found
	//ft_printf("no need for a new page\n");
		return (mark_block_as_allocated(list_head, padded(size_to_be_alloc), zone));
	}
}