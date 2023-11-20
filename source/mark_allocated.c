#include "malloc.h"

void	*mark_block_as_allocated(void *block, size_t size_to_be_allocated) {
	void	*working_pointer = block;
	size_t	payload_padded = ceilling_unsigned((double)size_to_be_allocated / (double)sizeof(size_t));
	size_t	left_over = *(size_t *)working_pointer - payload_padded - 2 * sizeof(size_t);
	
	if (block == NULL)
		return NULL;
	if (left_over > 2 * sizeof(size_t) + sizeof(t_list)) { // if the block can be split in a allocated block + a free block

	}
	else { // if it can only contain the payload

	}
	*((size_t *)working_pointer) += 1;

}

// le but ici est de diviser un gros block free en un block plein et un block free si possible si block free alors le ratacher correcteemnt