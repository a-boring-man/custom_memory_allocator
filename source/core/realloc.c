#include "malloc.h"

void	*realloc(void *ptr, size_t size) {
	t_memory_pointer	working_pointer;
	
	working_pointer.as_void = ptr;
	working_pointer.as_char -= (sizeof(size_t) + RED_ZONE_SIZE);
	
	ft_printf("realloc pointer is : -%p-", ptr);
	size_t	block_size = *working_pointer.as_sizeT;
	ft_printf(" the block size is : -%d-", block_size);
	size_t	data_size = block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;
	ft_printf(" the data size is : -%d-", data_size);

	if (size > data_size) { // need to check if a free space is next to the block and big enought else send back to malloc

	}
	else { // the size will be shrinked

	}
}