#include "malloc.h"
//								  PAGE
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
//								  PAGE
////////////////////////////////////////////////////////////////////////////
//S(S | T | 1(S |                                                         //
//I I | _ |   I |                                                         //
//Z Z | L |   Z |    F   R   E   E       B   L   O   C   K                //
//E E | I |   E |                                                         //
//  _ | S |   _ |                                                         //
//  T)| T |   T)|                                                         //
////////////////////////////////////////////////////////////////////////////


// 								FREE BLOCK
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
//								FREE BLOCK
////////////////////////////////////////////////////////////////////////////
//S(S | T |                                                          |S(S //
//I I | _ |                                                          |I I //
//Z Z | L |                                                          |Z Z //
//E E | I |                                                          |E E //
//  _ | S |                                                          |  _ //
//  T)| T |                                                          |  T)//
////////////////////////////////////////////////////////////////////////////
/**
 * @brief format the page to containe its size then a t_list, and formate the new free block
 * 
 * @param new_page pointer to the beginning of the page to be formated
 * @param page_size the size of the page to be formated
 * */
void	format_new_page(void *new_page, size_t page_size) {
	t_union	working_pointer;
	working_pointer.m_void = new_page;
	
	*(working_pointer.m_sizet) = page_size;
	working_pointer.m_sizet += 1;
	(working_pointer.m_tlist)->next = working_pointer.m_tlist;
	(working_pointer.m_tlist)->previous = working_pointer.m_tlist;
	working_pointer.m_tlist += 1;
	*(working_pointer.m_sizet) = 1;
	format_free_space((void *)(working_pointer.m_tlist + 1), page_size - sizeof(size_t) - sizeof(t_list));// todo !
}

void	format_free_space(void *new_page, size_t free_block_size) {
	t_union	working_pointer;
	working_pointer.m_void = new_page;
	
	*(working_pointer.m_sizet) = free_block_size;
	working_pointer.m_sizet += 1;
	(working_pointer.m_tlist)->next = working_pointer.m_tlist;
	(working_pointer.m_tlist)->previous = working_pointer.m_tlist;
	working_pointer.m_char += free_block_size - 2 * sizeof(size_t);
	*(working_pointer.m_sizet) = free_block_size;
	poison_block((void *)(working_pointer.m_char - free_block_size + sizeof(size_t) + sizeof(t_list)), free_block_size - 2 * sizeof(size_t) - sizeof(t_list), 0xcc); // todo !
} 