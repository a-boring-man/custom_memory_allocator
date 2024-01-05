#ifndef MALLOC_H
#define MALLOC_H

// ------------dependency----------

#include <stdio.h>
#include <unistd.h>
#include "ft_printf.h"

// ------------struct-------------

typedef struct s_list {
	struct s_list	*next;
	struct s_list	*previous;
}	t_list;

typedef struct s_zone {
	size_t	max_size;
	t_list	*free;
	t_list	*page;
	t_list	*next;
}	t_zone;

typedef union u_memory_pointer {
	void	*as_void;
	size_t	*as_sizeT;
	t_list	*as_Tlist;
	char	*as_char;
}	t_memory_pointer;

// -------------usefull define--------------

# ifdef RED_ZONE_DEBUG
	#define RED_ZONE_SIZE (2 * sizeof(size_t))
# else
	#define RED_ZONE_SIZE 0
# endif
# define MAX_SIZET (~(size_t)0)
# define MINIMUM_FREE_BLOCK_SIZE (2 * sizeof(size_t) + sizeof(t_list))
# define MINIMUM_ALLOCATED_BLOCK_SIZE (2 * sizeof(size_t) + 2 * RED_ZONE_SIZE)
# define PAGE_START_OVERHEAD (2 * sizeof(size_t) + sizeof(t_list))
# define PAGE_OVERHEAD (3 * sizeof(size_t) + sizeof(t_list))
# define MINIMUM_PAGE_SIZE (PAGE_OVERHEAD + 2 * RED_ZONE_SIZE)
# define RED_ZONE_COLOR 0xee
# define FREE_DELAY 256
# define unlikely(x)	(__builtin_expect(!!(x), 0))

// -------------global allocation variable-------------

extern t_zone grimoire[11];

// -------------mandatory function---------------

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem();

// -------------bonus function-------------

void	show_alloc_mem_ex();

/**
 * @brief simply set the size first bit at block location to poison 
 * 
 * @param block the memory location to be poison
 * @param size the size of the poison
 * @param poison the poison in char format
 */
void	poison_block(void *block, size_t size, char poison);

/**
 * @brief return the address of block but red_zonned according to the RED_ZONE_SIZE and RED_ZONE_COLOR variable
 * 
 * @param block the memory block to be redzone
 * @param size_allocated the size of the content of the block to not be redzoned so the padded size
 * @return void* 
 */
void	*red_zone(void *block, size_t size_allocated);

/**
 * @brief simply choose the right grimoir page for the soon to be allocated memory
 * 
 * @param size the size of the requested allocation
 * @return t_zone* the address of the t_zone corresponding to the allocated size
 */
t_zone	*choose_the_right_page(size_t size);

/**
 * @brief find the free block with the least amount of left over for the allocation
 * 
 * @param size_to_be_alloc the raw size to be allocated into memory
 * @param zone the correct grimoir t_zone for this allocation size
 * @return void* a pointer to the begginning of the allocated space ready to be writen on
 */
void	*best_fit(size_t size_to_be_alloc, t_zone *zone);

/**
 * @brief always chosse the next free space in memory, supper fast but lead to lot of splinter in the begginning of the address space
 * 
 * @param size_to_be_alloc the raw size to be allocated into memory
 * @param zone the correct grimoir t_zone for this allocation size
 * @return void* a pointer to the begginning of the allocated space ready to be writen on
 */
void	*next_fit(size_t size_to_be_alloc, t_zone *zone);

/**
 * @brief find the first free block big enough to accept the size requested
 * 
 * @param size_to_be_alloc the raw size to be allocated into memory
 * @param zone the correct grimoir t_zone for this allocation size
 * @return void* a pointer to the begginning of the allocated space ready to be writen on
 */
void	*first_fit(size_t size_to_be_alloc, t_zone *zone);

/**
 * @brief a bonus function that help reduce splinter at the beginning of memory by coalescing freeblock together
 * 
 * @param ptr the addres given to the free function
 * @param zone the correct zone for the malloced size
 */
void	coalescing(void *ptr, t_zone *zone);

// -------------memory function----------------

/**
 * @brief Request a new memory page to the kernel using mmap()
 * 
 * @param zone the zone selected for the size requested
 * @param size the raw size requested, only matter if the size is > 8192 bit
 * @return void* return a pointer to the begginning of the newly created page
 */
void	*create_page(t_zone *zone, size_t size);

/**
 * @brief write the nessessary meta data in the given page
 * 
 * @param new_page a pointer to the page being written to
 * @param page_size the correct page size
 */
void	format_new_page(void *new_page, size_t page_size);

/**
 * @brief mark a free block as allocated, changing the next free block pointer and the free t_list inside zone if nessessary
 * 
 * @param block the t_list part of a free block
 * @param size_to_be_allocated the padded size to be allocated
 * @param zone the zone conatinning the free block
 * @return void* a pointer redzonned to the written part of the newly allocated block
 */
void	*mark_block_as_allocated(t_list *block, size_t size_to_be_allocated, t_zone *zone);

/**
 * @brief use if the block can be realloc on place, so only changing the free block next to it
 * 
 * @param block_beginning self explicit
 * @param zone the correct zone for the block
 * @param size the padded size to realloc
 */
void	mark_block_as_allocated_from_realloc(void *block_beginning, t_zone *zone, size_t size);

/**
 * @brief mark a region of memory as free
 * 
 * @param block the pointer given as argument to free
 * @param zone the corrected zone of the maloced block
 */
void	mark_block_as_free(void *block, t_zone *zone);

// -------------t_list function----------------

/**
 * @brief the general function to call to add a block to a t_list
 * 
 * @param new_block the block to be added
 * @param list_head the address of the t_list to be modified
 */
void	add_block_to_t_list(t_list *new_block, t_list **list_head);

/**
 * @brief remove a block from a t_list
 * 
 * @param block the block to be removed from the t_list
 * @param head the address of the list_head to be modified
 */
void	remove_block_from_t_list(t_list *block, t_list **head);

/**
 * @brief check all element a page containning t_list and remove them if the condition function return true on an element
 * 
 * @param list_head the address of the beginning of the list
 * @param condition_function in that specifique case a function that determini if the page is only composed of on big free block may be improved to also recognize a case where we don't coalesc block together
 */
void	remove_page_if(t_list **list_head, int (*condition_function)(void *));

// -------------helper function-----------------

size_t	min(size_t a, size_t b);

/**
 * @brief give back a rounded value to the nearest rounding
 * 
 * @param x the value to be rounded
 * @param rounding rounded to rounding
 * @return size_t the rounded value
 */
size_t	ceilling_unsigned(size_t x, size_t rounding);

/**
 * @brief return the padded (rounded to the upper sizeof(size_t)) size
 * 
 * @param size size to be padded
 * @return size_t the returned size
 */
size_t	padded(size_t size);

/**
 * @brief calculate the created page size based on the size to be malloc and the page size for the given zone
 * 
 * @param zone 
 * @param size_to_be_malloc 
 * @return size_t 
 */
size_t	determine_page_size(t_zone *zone, size_t size_to_be_malloc);

// -----------------------------------------

void	debug_hexa(void *memory_location, size_t size);

#endif