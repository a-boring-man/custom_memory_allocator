#include "malloc.h"

void	*mark_block_as_allocated(void *block, size_t size_to_be_allocated) {
	void	*working_pointer = block;
	size_t	payload_padded = ceilling_unsigned((double)size_to_be_allocated / (double)sizeof(size_t));
	
	if (block == NULL)
		return NULL;
	*((size_t *)working_pointer) += 1;

}

// le but ici est de diviser un gros block free en un block plein et un block free si possible si block free alors le ratacher correcteemnt