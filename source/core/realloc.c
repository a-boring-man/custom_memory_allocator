#include "malloc.h"

void	*ft_memcpy(void *to, void *from, size_t size)
{
	size_t	i;

	i = 0;
	ft_printf("my memcpy \n");
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
	
	ft_printf("!!!!!!!!!!!realloc pointer is : -%p- and size : -%d-", ptr, size);
	size_t	left_block_size = *working_pointer.as_sizeT & -2;
	ft_printf(" the block size is : -%d-", left_block_size);
	size_t	data_size = left_block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;
	ft_printf(" the data size is : -%d-\n", data_size);
	if (padded(size) == data_size) {
		return ptr;
	}

	t_zone	*block_zone = choose_the_right_page(data_size); // get the zone of the data to be realloc
	t_zone	*new_block_zone = choose_the_right_page(size); // get the zone of the newly size malloc
	int need_to_be_moved = block_zone != new_block_zone; // check to see if the content need to be moved

	if (need_to_be_moved || size > data_size) { // need to check if a free space is next to the block and big enought else send back to malloc
		working_pointer.as_char += left_block_size; // move the pointer to the next block to check if it's free
		if (!(*working_pointer.as_sizeT & 1) && *working_pointer.as_sizeT >= (padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE) && !need_to_be_moved) { // if block is free and big enough and data doesn't need to be moved
		 	ft_printf("in realloc first case\n");
			size_t	right_block_size = *working_pointer.as_sizeT; // store the right block size
			remove_block_from_t_list((t_list *)(working_pointer.as_sizeT + 1), &(block_zone->free)); // remove the right free block from the list
			working_pointer.as_char += (right_block_size - sizeof(size_t)); // go to the end of the right block
			*working_pointer.as_sizeT = right_block_size + left_block_size; // put the lenght of the two block and mark it as free;
			working_pointer.as_char -= (*working_pointer.as_sizeT - sizeof(size_t)); // go back to the beginning
			*working_pointer.as_sizeT = right_block_size + left_block_size; // set the left size to the size of the two block as free block
			mark_block_as_allocated_from_realloc(working_pointer.as_void, block_zone, padded(size));
			return (ptr);
		}
		else { // call malloc then copy if need to be moved
			ft_printf("in realloc second case\n");
			void	*new_pointer = malloc(size);
			ft_memcpy(new_pointer, ptr, min(data_size, padded(size))); // copy the old content
			ft_printf("just before debug in realloc new_pointer is %p and red_size is %d\n", new_pointer, RED_ZONE_SIZE);
			debug_hexa((size_t *)new_pointer - 7, 50);
			free(ptr);
			return new_pointer;
		}
	}
	else { // the size will be shrinked
		
		ft_printf("in realloc third case\n");
		int should_be_split = data_size - padded(size) >= MINIMUM_FREE_BLOCK_SIZE; // if there is enough space to put a new free_block
		size_t	left_over = data_size - padded(size);
		ft_printf("left_over is %d\n", left_over);
		working_pointer.as_char += left_block_size; // move the wp to the next block to see if it's a free block
		int	has_a_free_block_behind = !(*working_pointer.as_sizeT & 1); // self explenatory
		size_t	free_block_size = 0;

		if (has_a_free_block_behind) {// free block behind can be split and merge to the free block
		ft_printf("in realloc fourth case\n");
			free_block_size = *working_pointer.as_sizeT & -2; // store the size of the free block
			ft_printf("realloc found free_block size : %d\n", free_block_size);
			remove_block_from_t_list((t_list *)(working_pointer.as_sizeT + 1), &(block_zone->free)); // remove it from the list
			working_pointer.as_char -= (left_over); // go back to the now new beguinning of the freeblock
			*working_pointer.as_sizeT = free_block_size + left_over; // put the now bigger size in it
			ft_printf("what is writen is %d\n", *working_pointer.as_sizeT);
			working_pointer.as_char += *working_pointer.as_sizeT - sizeof(size_t); // move to the end of the free block
			*working_pointer.as_sizeT = free_block_size + left_over; // modify the size of the block
			ft_printf("new_free_block size : %d\n", free_block_size + left_over);
			working_pointer.as_char -= (*working_pointer.as_sizeT - 2 * sizeof(size_t)); // go back to the t_list part
			debug_hexa(working_pointer.as_void, 2);
			add_block_to_t_list(working_pointer.as_Tlist, &(block_zone->free)); // readd the block to the list of free block
			working_pointer.as_sizeT -= 2; // move to the now end of the malloced block
			*working_pointer.as_sizeT = padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE + 1; // write the new block size
			working_pointer.as_char -= (padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE - sizeof(size_t)); // go to the begginning of the block
			*working_pointer.as_sizeT = padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE + 1; // write the new block size
			ft_printf("HERE\\\\\\\\\\\\\\\\\\\\\\\n");
			return (red_zone((void *)(working_pointer.as_char + sizeof(size_t)), padded(size))); // re redzone the block
		}
		else if (should_be_split) {//can be split
			ft_printf("fith case \n");
			
		return NULL;
		}
		else {// nothing to be done

		return NULL;
		}
	}
	return NULL;
}