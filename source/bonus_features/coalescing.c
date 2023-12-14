#include "malloc.h"

static	void	coalescing_left(void *block_ptr) { // return the left side of a free block coalesced with a potential left free block
	t_memory_pointer	working_pointer;
	size_t	left_block_size;
	working_pointer.as_void = block_ptr;
	working_pointer.as_sizeT -= 1;

	if (*working_pointer.as_sizeT & 1) { // begginning of the page or allocated block
		return (block_ptr); // return the original pointer;
	}
	working_pointer.as_char -= (*(working_pointer.as_sizeT) - sizeof(size_t)); // point to the begginning of the left free block to change it's value
	left_block_size = *working_pointer.as_sizeT; // store the original left block size
	*working_pointer.as_sizeT += *(size_t *)block_ptr; // change the first left block size marker
	working_pointer.as_char += *working_pointer.as_sizeT - sizeof(size_t); // go to the end marker to change it
	*working_pointer.as_sizeT += left_block_size; // change the end marker to the now true size of the block
}

static	void	coalescing_right(t_memory_pointer working_pointer, t_zone *zone) {

}

void	coalescing(void *ptr, t_zone *zone) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = ptr;
	
	working_pointer.as_char -= (RED_ZONE_SIZE + sizeof(size_t)); // pointer to the size of the block
	working_pointer.as_void = coalescing_left(working_pointer.as_void);
	working_pointer.as_void = coalescing_right(working_pointer.as_void, zone);
}