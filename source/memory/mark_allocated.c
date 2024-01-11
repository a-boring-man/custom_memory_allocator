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
	//ft_printf("AAAAAAAAAAAAAAaaathe arg block : -%p-\n", block);
	t_memory_pointer	working_pointer;
	working_pointer.as_Tlist = block;
	working_pointer.as_sizeT -= 1; // go back to the beginning of the block
	//int	is_alone = 0;
	//ft_printf("entering mark alloc, working pointer is : -%p-\n", working_pointer.as_void);
	size_t	block_size = (*working_pointer.as_sizeT & -2); //  get the block size
	//ft_printf("BBBBBBBBBBBBB the detected block size = -%u-\n", block_size);
	size_t	left_over = block_size - (size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE); //  calculate the left over
	//ft_printf("CCCCCCCCCCCC the left over size = -%u-\n", left_over);

	//ft_printf("going to marked alloc block : -%p-\n", block);
	
	if (left_over >= MINIMUM_FREE_BLOCK_SIZE) { // if the block can be split in a allocated block + a free block
	//ft_printf("in split block in marked alloc\n");

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
	//ft_printf("FFFFFFFFFFFF working_pointer.as_void: -%p-\n", working_pointer.as_void);




















	// 	t_list copy = *(block);
	// 	ft_printf("CCCCCCCCCCCDDDDDDDDDDD copy contain next : -%p- previous : -%p-\n", copy.next, copy.previous);
	// 	if (block->next == block) {
	// 		is_alone = 1;
	// 	}
	// 	ft_printf("DDDDDDDDddd is alone : -%d-\n", is_alone);
	// ft_printf("the copy is : next -%p- previous -%p-\n", copy.next, copy.previous);
	// 	*(working_pointer.as_sizeT) = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1; // change the first size to be the allocated block size
	// 	working_pointer.as_char += size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE - sizeof(size_t); // move the pointer to the end of the allocated block
	// 	ft_printf("FFFFFFFFFFFF working_pointer.as_void: -%p-\n", working_pointer.as_void);
	// ft_printf("end of marked alloc returning : -%p-\n", block);
	// 	*(working_pointer.as_sizeT) = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1; // write the new size here
	// 	working_pointer.as_sizeT += 1; // move to the free space after
	// 	ft_printf("FFFFFFFFFFFF working_pointer.as_void: -%p-\n", working_pointer.as_void);
	// 	*(working_pointer.as_sizeT) = left_over; // change the free space size
	// 	working_pointer.as_sizeT += 1; // move to the t_list part
	// 	ft_printf("FFFFFFFFFFFF working_pointer.as_void: -%p-\n", working_pointer.as_void);
	// 	*(working_pointer.as_Tlist) = copy; // copt the t_list
	// 	ft_printf("EEEEEEEEEEEEE size to be allocated : -%d-\n", size_to_be_allocated);
	// 	zone->next = working_pointer.as_Tlist;
	// 	if (!is_alone) {
	// 		(working_pointer.as_Tlist)->next->previous = working_pointer.as_Tlist;
	// 		(working_pointer.as_Tlist)->previous->next = working_pointer.as_Tlist;
	// 	}
	// 	else { // is alone in zone->free
	// 		zone->free = working_pointer.as_Tlist;
	// 		ft_printf("----------->>>>>>the pointer is at address : -%p-\n", working_pointer.as_void);
	// 		(working_pointer.as_Tlist)->next = working_pointer.as_Tlist;
	// 		(working_pointer.as_Tlist)->previous = working_pointer.as_Tlist;
	// 	}
	// 	working_pointer.as_char += *(working_pointer.as_sizeT - 1) - 2 * sizeof(size_t);
	// 	*(working_pointer.as_sizeT) = left_over;
	//debug_hexa((size_t *)block - 5, 14);
		return red_zone(block, size_to_be_allocated);
	}
	else { // if it can only contain the payload
		remove_block_from_t_list(block, &(zone->free));
		zone->next = NULL;
		*(working_pointer.as_sizeT) += 1;
		working_pointer.as_char += *(working_pointer.as_sizeT) - 1 - sizeof(size_t);
		*(working_pointer.as_sizeT) += 1;
	ft_printf("end of marked alloc\n");
		return red_zone(block, size_to_be_allocated);
	}
	ft_printf("end of marked alloc\n");
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