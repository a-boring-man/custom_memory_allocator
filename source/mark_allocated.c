#include "malloc.h"


/**
 * @brief the goql of this function is to mark a chunck of memory as allocated
 * 
 * @param block is the addres of a t_list in a free block
 * @param size_to_be_allocated is the size to be allocated taking into acount the padding if needed
 * @param zone is the t_zone where the free block is.
 * @return void* a pointer to the beginning of the marked allocated block where the user can write
 */
void	*mark_block_as_allocated(t_list *block, size_t size_to_be_allocated, t_zone *zone) {
	void	*working_pointer = block - sizeof(size_t);
	size_t	block_size = *((size_t *)working_pointer);
	// size_t	payload_padded = ceilling_unsigned((double)size_to_be_allocated / (double)sizeof(size_t)); to be copied in the caller of this function
	size_t	left_over = block_size - (size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE);
	
	if (block == NULL)
		return NULL;
	if (left_over >= MINIMUM_FREE_BLOCK_SIZE) { // if the block can be split in a allocated block + a free block
		t_list copy = *((t_list *)working_pointer + 1);
		*((size_t *)working_pointer) = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1;
		working_pointer += (size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE - sizeof(size_t));
		*((size_t *)working_pointer) = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1;
		working_pointer += sizeof(size_t);
		*((size_t *)working_pointer) = left_over;
		working_pointer += sizeof(size_t);
		*((t_list *)working_pointer) = copy;
		((t_list *)working_pointer)->next->previous = working_pointer;
		((t_list *)working_pointer)->previous->next = working_pointer;
		working_pointer += *((size_t *)(working_pointer - sizeof(size_t))) - 2 * sizeof(size_t);
		*((size_t *)working_pointer) = left_over;
		return red_zone(block, size_to_be_allocated);
	}
	else { // if it can only contain the payload
		remove_block_from_t_list(block, &(zone->free));
		*((size_t *)working_pointer) += + 1;
		working_pointer += (*((size_t *)working_pointer) - 1 - sizeof(size_t));
		*((size_t *)working_pointer) += 1;
	}
	*((size_t *)working_pointer) += 1;
	return NULL;
}

// le but ici est de diviser un gros block free en un block plein et un block free si possible si block free alors le ratacher correcteemnt