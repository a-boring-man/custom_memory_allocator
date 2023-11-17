#include "malloc.h"

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
//									|
//									|
//									|
//									|
//									|
//									V

////////////////////////////////////////////////////////////////////////////
//S(S | T |                                                               //
//I I | _ |                                                               //
//Z Z | L |     P   A   Y   L   O   A   D                                 //
//E E | I |                                                               //
//  _ | S |                                                               //
//  T)| T |                                                               //
////////////////////////////////////////////////////////////////////////////

void	format_free_space(void *new_page, size_t free_block_size);

void	format_new_page(void *new_page, size_t page_size) {
	void	*working_pointer = new_page;
	
	*working_pointer = page_size;
	working_pointer += sizeof(size_t);
	(t_list *)working_pointer->next = working_pointer;
	(t_list *)working_pointer->previous = working_pointer;
	format_free_space(working_pointer + sizeof(t_list), page_size - sizeof(size_t) - sizeof(t_list));
}

void	format_free_space(void *new_page, size_t free_block_size) {
	void	*working_pointer = new_page;
	
	*working_pointer = free_block_size;
	working_pointer += sizeof(size_t);
	(t_list *)working_pointer->next = working_pointer;
	(t_list *)working_pointer->previous = working_pointer;
} 