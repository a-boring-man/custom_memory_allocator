#include "malloc.h"

void	*ft_memcpy(void *to, void *from, size_t size)
{
	size_t	i;

	i = 0;
	if (!to && ! from)
		return (NULL);
	while (i < size)
	{
		*(char *)(to + i) = *(char *)(from + i);
		i++;
	}
	return (to);
}

void	*realloc(void *ptr, size_t size) {
	if (ptr == NULL) { // if ptr is NULL the call is equivalent to malloc(size) regardless of size
		return (malloc(size));
	}
	if (size == 0) { // if size == 0 the call is equivalent to free(ptr) and NULL can be returned
		free(ptr);
		return NULL;
	}

	t_memory_pointer	working_pointer;
	
	working_pointer.as_void = ptr;
	working_pointer.as_char -= (sizeof(size_t) + RED_ZONE_SIZE); // put the working_pointer to the size of the block
	
	ft_printf("realloc pointer is : -%p- and size : -%d-", ptr, size);
	size_t	left_block_size = *working_pointer.as_sizeT & -2;
	ft_printf(" the block size is : -%d-", left_block_size);
	size_t	data_size = left_block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;
	ft_printf(" the data size is : -%d-\n", data_size);

	t_zone	*block_zone = choose_the_right_page(data_size); // get the zone of the data to be realloc
	t_zone	*new_block_zone = choose_the_right_page(size); // get the zone of the newly size malloc
	int need_to_be_moved = block_zone != new_block_zone; // check to see if the content need to be moved

	return NULL;
	if (need_to_be_moved || size > data_size) { // need to check if a free space is next to the block and big enought else send back to malloc
		working_pointer.as_char += left_block_size; // move the pointer to the next block to check if it's free
		if (!(*working_pointer.as_sizeT & 1) && *working_pointer.as_sizeT >= (padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE) && !need_to_be_moved) { // if block is free and big enough and data doesn't need to be moved
			size_t	right_block_size = *working_pointer.as_sizeT; // store the right block size
			remove_block_from_t_list((t_list *)(working_pointer.as_sizeT + 1), &(block_zone->free)); // remove the right free block from the list
			working_pointer.as_char += (right_block_size - sizeof(size_t)); // go to the end of the right block
			*working_pointer.as_sizeT = right_block_size + left_block_size; // put the lenght of the two block and mark it as free;
			working_pointer.as_char -= (*working_pointer.as_sizeT - sizeof(size_t)); // go back to the beginning
			*working_pointer.as_sizeT = right_block_size + left_block_size; // set the left size to the size of the two block as free block
			mark_block_as_allocated_from_realloc(working_pointer.as_void, block_zone, padded(size));
		}
		else { // call malloc then copy if need to be moved 
			void	*new_pointer = malloc(size);
			ft_memcpy(new_pointer, ptr, data_size); // copy the old content
		}
	}
	else { // the size will be shrinked
		
		//int should_be_split =
	}
}