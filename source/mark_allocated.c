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



/**
 * @brief the goal of this function is to mark a chunck of memory as allocated
 * 
 * @param block is the addres of a t_list in a free block
 * @param size_to_be_allocated is the size to be allocated taking into acount the padding if needed
 * @param zone is the t_zone where the free block is.
 * @return void* a pointer to the beginning of the marked allocated block where the user can write
 */
void	*mark_block_as_allocated(t_list *block, size_t size_to_be_allocated, t_zone *zone) {
	void	*working_pointer = block - sizeof(size_t);
	size_t	block_size = *((size_t *)working_pointer);
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
		*((size_t *)working_pointer) += 1;
		working_pointer += (*((size_t *)working_pointer) - 1 - sizeof(size_t));
		*((size_t *)working_pointer) += 1;
		return red_zone(block, size_to_be_allocated);
	}
	*((size_t *)working_pointer) += 1;
	return NULL;
}