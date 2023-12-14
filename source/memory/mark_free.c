#include "malloc.h"

void	mark_block_as_free(void *block) {
	t_memory_pointer	working_pointer;
	
	working_pointer.as_void = block;
	working_pointer.as_char -= (RED_ZONE_SIZE + sizeof(size_t)); // set the address of WP to the beggining of the block
	*working_pointer.as_sizeT = *working_pointer.as_sizeT & -2; // mark as free
	working_pointer.as_char += *working_pointer.as_sizeT - sizeof(size_t); // move to the end of the block
	*working_pointer.as_sizeT = *working_pointer.as_sizeT & -2; // mark as free
}