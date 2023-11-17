#include "malloc.h"

/**
 * @brief write meta data next to the data to be malloced
 * 
 * @param block the very begginning of the free block to be malloced
 * @param block_size the size of the malloced content
 */
void	format_allocated_block(void	*block, size_t malloc_size) {
	*((size_t *)block) = malloc_size + 1;
}

void	*best_fit(size_t size_to_be_alloc, t_zone *zone) {

}

void	*next_fit(size_t size_to_be_alloc, t_zone *zone) {}

void	*first_fit(size_t size_to_be_alloc, t_zone *zone) {
	t_list	*list_head = zone->free;

	if (list_head == NULL) { // empty free list
		size_t page_size;
		if (zone->max_size == 0) {
			page_size = ceilling_unsigned((double)(size + 24 + 2 * RED_ZONE_SIZE) / (double)computer_page_size) * computer_page_size; // compute the alocated page TRUE size
		}
		void	*new_page = create_page(zone, size_to_be_alloc 
			#ifdef RED_ZONE_SIZE 
			+ 2 * RED_ZONE_SIZE 
			#endif
			);
		format_new_page(new_page, );
	}
}