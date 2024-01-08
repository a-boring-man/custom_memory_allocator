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
//S(S | T | 1(S |                                                    |1(S //
//I I | _ |   I |                                                    |  I //
//Z Z | L |   Z |    F   R   E   E       B   L   O   C   K           |  Z //
//E E | I |   E |                                                    |  E //
//  _ | S |   _ |                                                    |  _ //
//  T)| T |   T)|                                                    |  T)//
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

static void	format_free_space(void *new_page, size_t free_block_size) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = new_page;
	
	//ft_printf("page begginning in format free block : -%p-\n", new_page);
	//ft_printf("free block size : -%d-\n", free_block_size);
	*(working_pointer.as_sizeT) = free_block_size;
	working_pointer.as_sizeT += 1;
	(working_pointer.as_Tlist)->next = working_pointer.as_Tlist;
	(working_pointer.as_Tlist)->previous = working_pointer.as_Tlist;
	working_pointer.as_char += free_block_size - 2 * sizeof(size_t);
	*(working_pointer.as_sizeT) = free_block_size;
	//ft_printf("free space end : -%p-\n", working_pointer.as_char);
	debug_hexa((void *)((char *)new_page - PAGE_START_OVERHEAD), (*(size_t *)((char *)new_page - PAGE_START_OVERHEAD)) / sizeof(size_t));
	poison_block((void *)(working_pointer.as_char - free_block_size + 2 * sizeof(size_t) + sizeof(t_list)), free_block_size - (MINIMUM_FREE_BLOCK_SIZE), 0xcc);
} 

void	format_new_page(void *new_page, size_t page_size) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = new_page;
	
	//ft_printf("new page starting address : -%p-\n", new_page);
	*(working_pointer.as_sizeT) = page_size;
	working_pointer.as_sizeT += 1;
	(working_pointer.as_Tlist)->next = working_pointer.as_Tlist;
	(working_pointer.as_Tlist)->previous = working_pointer.as_Tlist;
	working_pointer.as_Tlist += 1;
	*(working_pointer.as_sizeT) = 1;
	working_pointer.as_char += page_size - 2 * sizeof(size_t) - sizeof(t_list);
	*(working_pointer.as_sizeT) = 1;
	//ft_printf("page end : -%p-\n", working_pointer.as_char);
	format_free_space((char *)new_page + PAGE_START_OVERHEAD, page_size - (PAGE_OVERHEAD));
}