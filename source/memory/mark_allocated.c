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
	working_pointer.as_sizeT -= 1;
	size_t	block_size = *(working_pointer.as_sizeT);
	size_t	left_over = block_size - (size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE);
	
	if (block == NULL)
		return NULL;
	if (left_over >= MINIMUM_FREE_BLOCK_SIZE) { // if the block can be split in a allocated block + a free block
		t_list copy = *(block);
		*(working_pointer.as_sizeT) = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1;
		working_pointer.as_char += size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE - sizeof(size_t);
		*(working_pointer.as_sizeT) = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1;
		working_pointer.as_sizeT += 1;
		*(working_pointer.as_sizeT) = left_over;
		working_pointer.as_sizeT += 1;
		*(working_pointer.as_Tlist) = copy;
		zone->next = working_pointer.as_Tlist;
		if (working_pointer.as_Tlist->next->previous != working_pointer.as_Tlist) { // the free list had NOT only one element
			working_pointer.as_Tlist->next->previous = working_pointer.as_Tlist;
			working_pointer.as_Tlist->previous->next = working_pointer.as_Tlist;
		}
		else { // free list had one element
			zone->free = working_pointer.as_Tlist;
		}
		working_pointer.as_char += *(working_pointer.as_sizeT - 1) - 2 * sizeof(size_t);
		*(working_pointer.as_sizeT) = left_over;
		return red_zone(block, size_to_be_allocated);
	}
	else { // if it can only contain the payload
		remove_block_from_t_list(block, &(zone->free));
		zone->next = NULL;
		*(working_pointer.as_sizeT) += 1;
		working_pointer.as_char += *(working_pointer.as_sizeT) - 1 - sizeof(size_t);
		*(working_pointer.as_sizeT) += 1;
		return red_zone(block, size_to_be_allocated);
	}
}