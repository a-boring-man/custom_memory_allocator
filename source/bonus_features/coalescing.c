#include "malloc.h"

static	void	coalescing_left(void **block_ptr, t_zone *zone) { // return the left side of a free block coalesced with a potential left free block
	t_memory_pointer	working_pointer;
	size_t	left_block_size;
	working_pointer.as_void = *block_ptr;
	working_pointer.as_sizeT -= 1; // put the pointer to the end of the previous block

	if (*working_pointer.as_sizeT & 1) { // begginning of the page or allocated block
		ft_printf("there is no free block on the left\n");
		return ;
	}
	left_block_size = *working_pointer.as_sizeT; // store the original left block size
	remove_block_from_t_list((t_list *)((size_t *)(*block_ptr) + 1), &(zone->free));
	working_pointer.as_char += *((size_t *)(*block_ptr)); // go to the end of the block to change it's marked size
	*working_pointer.as_sizeT += left_block_size; // change the end block marker
	working_pointer.as_char -= (*working_pointer.as_sizeT - sizeof(size_t)); // go to the first marker to change it
	*working_pointer.as_sizeT += *((size_t *)(*block_ptr)); // change the first marker to the now true size of the block
	working_pointer.as_sizeT += 1; // put the pointer to the t_list part
	remove_block_from_t_list(working_pointer.as_Tlist, &(zone->free)); // remove the big block from the free list
	add_block_to_t_list(working_pointer.as_Tlist, &(zone->free)); // re add the block back so it's first on the list to avoid splinter at the beginning of list
	*block_ptr = working_pointer.as_void; // srt the block address to the very begginning of the coalesced block
}

static	void	coalescing_right(void *block_ptr, t_zone *zone) {
	t_memory_pointer	working_pointer;
	size_t	right_block_size;
	working_pointer.as_void = block_ptr;
	working_pointer.as_char += *((size_t *)(block_ptr)); // put the pointer to the begginnin of the right block

	if (*working_pointer.as_sizeT & 1) { // if the right block is allocated
		ft_printf("no space on the right side");
		return ;
	}
	right_block_size = *working_pointer.as_sizeT; // store the right part
	working_pointer.as_sizeT += 1; // move to the right block t_list part
	remove_block_from_t_list(working_pointer.as_Tlist, &(zone->free)); // remove the right free block from the list
	working_pointer.as_sizeT -= 1; // move back to the size of the right block
	working_pointer.as_char += *working_pointer.as_sizeT - sizeof(size_t); // jump to the end
	*working_pointer.as_sizeT += *((size_t *)(block_ptr)); // add the size of the left block to the end of the right block
	working_pointer.as_char -= (*working_pointer.as_sizeT - sizeof(size_t)); // move to the begginning of the left block
	*working_pointer.as_sizeT += right_block_size; // change it's value to be the sum of the two block lenght
	working_pointer.as_sizeT += 1; // move to the t_list part to change the free list accordingly
	remove_block_from_t_list(working_pointer.as_Tlist, &(zone->free)); // remove the big block from the free list
	add_block_to_t_list(working_pointer.as_Tlist, &(zone->free)); // re add the block back so it's first on the list to avoid splinter at the beginning of list
}

void	coalescing(void *ptr, t_zone *zone) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = ptr;
	
	working_pointer.as_char -= (RED_ZONE_SIZE + sizeof(size_t)); // pointer to the size of the block
	coalescing_left(&working_pointer.as_void, zone);
	coalescing_right(working_pointer.as_void, zone);
}