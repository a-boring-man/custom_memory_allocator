#include "malloc.h"
//								FREE BLOCK BEFORE
////////////////////////////////////////////////////////////////////////////
//S(S | T |                                                          |S(S //
//I I | _ |                                                          |I I //
//Z Z | L |                                                          |Z Z //
//E E | I |                                                          |E E //
//  _ | S |                                                          |  _ //
//  T)| T |                                                          |  T)//
////////////////////////////////////////////////////////////////////////////
//									|
//									|
//									|
//									|
//									|
//									V
//								SPLITED_BLOCK
////////////////////////////////////////////////////////////////////////////
//S(S |R Z|                              |R Z|S(S |S(S | T |         |S(S //
//I I |E O|                              |E O|I I |I I | _ |         |I I //
//Z Z |D N|	P A Y L O A D                |D N|Z Z |Z Z | L |FREE ZONE|Z Z //
//E E |  E|                              |  E|E E |E E | I |         |E E //
//  _ |   |                              |   |  _ |  _ | S |         |  _ //
//  T)|   |                              |   |  T)|  T)| T |         |  T)//
////////////////////////////////////////////////////////////////////////////
//									|
//									OR
//									|
//									V
//								WASTED_BLOCK
////////////////////////////////////////////////////////////////////////////
//S(S |R Z|                                                     |R Z|S(S |//
//I I |E O|                                                     |E O|I I |//
//Z Z |D N|	P A Y L O A D                                       |D N|Z Z |//
//E E |  E|                                                     |  E|E E |//
//  _ |   |                                                     |   |  _ |//
//  T)|   |                                                     |   |  T)|//
////////////////////////////////////////////////////////////////////////////

void	*mark_block_as_allocated(t_list *block, size_t size_to_be_allocated, t_zone *zone) { // should be use with the padded allocated size
	t_memory_pointer	working_pointer;
	working_pointer.as_Tlist = block;
	
	working_pointer.as_sizeT -= 1; // go back to the beginning of the block
	size_t	block_size = (*working_pointer.as_sizeT & -2); //  get the block size
	size_t	left_over = block_size - (size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE); //  calculate the left over
	
	if (left_over >= MINIMUM_FREE_BLOCK_SIZE) { // if the block can be split in a allocated block + a free block

		remove_block_from_t_list(block, &(zone->free)); // remove the block from the free list
		*working_pointer.as_sizeT = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1;
		working_pointer.as_char += size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE - sizeof(size_t);
		*working_pointer.as_sizeT = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1;
		working_pointer.as_sizeT += 1; // move to the new free block location
		*working_pointer.as_sizeT = left_over;
		working_pointer.as_sizeT += 1; // move to the t_list part
		working_pointer.as_Tlist->next = working_pointer.as_Tlist;
		working_pointer.as_Tlist->previous = working_pointer.as_Tlist;
		add_block_to_t_list(working_pointer.as_Tlist, &(zone->free));
		working_pointer.as_char += left_over - 2 * sizeof(size_t); // move to the end
		*working_pointer.as_sizeT = left_over;
		return red_zone(block, size_to_be_allocated);
	}
	else { // if it can only contain the payload
		remove_block_from_t_list(block, &(zone->free));
		*(working_pointer.as_sizeT) += 1;
		working_pointer.as_char += *(working_pointer.as_sizeT) - 1 - sizeof(size_t);
		*(working_pointer.as_sizeT) += 1;
		return red_zone(block, size_to_be_allocated);
	}
}

void	mark_block_as_allocated_from_realloc(void *block_beginning, t_zone *zone, size_t size) { // here size is the new size requested by realloc padded. here i already remove the free block from the list
	t_memory_pointer	working_pointer;
	working_pointer.as_void = block_beginning;
	size_t	block_size = *working_pointer.as_sizeT & -2; // store the block size for later use
	size_t	needed_size = size + MINIMUM_ALLOCATED_BLOCK_SIZE;
	int		should_be_split = block_size - needed_size >= MINIMUM_FREE_BLOCK_SIZE; // determini if the block shoul be split into a allocated block and a free block
	if (should_be_split) {
		size_t	left_over = block_size - needed_size;
		*working_pointer.as_sizeT = needed_size + 1; // put the true needed size
		working_pointer.as_char += (needed_size - sizeof(size_t)); // move the pointer to the end of the nwely marked allocated block
		*working_pointer.as_sizeT = needed_size + 1; // mark the end of the block
		working_pointer.as_sizeT += 1; // move to the free section
		*working_pointer.as_sizeT = left_over; // write the size of the free block
		working_pointer.as_char += (left_over - sizeof(size_t)); // go to the end of the free block
		*working_pointer.as_sizeT = left_over; // store the end marker
		working_pointer.as_char -= (*working_pointer.as_sizeT - 2 * sizeof(size_t)); // return at the t_list emplacement
		add_block_to_t_list(working_pointer.as_Tlist, &(zone->free)); // add the last block to the free list
		// todo add coalescing in bonus version, nothing to do here because the free block is already a unique big block before either the end of the page or an other allocated block
		red_zone((char *)block_beginning + sizeof(size_t), size); // recollor the block
	}
	else { // cant split it
		*working_pointer.as_sizeT = block_size + 1; // mark bothe end of the block
		working_pointer.as_char += (block_size - sizeof(size_t));
		*working_pointer.as_sizeT = block_size + 1;
		working_pointer.as_char -= (block_size - 2 * sizeof(size_t)); // go back to the t_list part of the allocated block
		red_zone(working_pointer.as_Tlist, size);
	}
}