#include "malloc.h"

void	free(void *ptr) {
	size_t	block_size;
	size_t	data_size;
	
	if (ptr == NULL) {
		return;
	}

	block_size = *((size_t *)ptr - 1) & -2;
	data_size = block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;
	t_zone	*zone = choose_the_right_page(data_size);
	
	ft_printf("wanting to free the block at : -%p- of size : -%d- and of true size : -%d-\n", ptr, block_size, data_size);
	
	mark_block_as_free(ptr, block_size, data_size);
	coalescing(ptr, zone);
	
	(void)ptr;
	int pff = write(1, "coucou\n", 7);
	(void)pff;
}