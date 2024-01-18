#ifndef MALLOC_H
#define MALLOC_H

// ------------dependency----------

#include <stdio.h>
#include <unistd.h>
#include "ft_printf.h"
#include <fcntl.h>
#include <pthread.h>

// ------------struct-------------

typedef struct s_list {
	struct s_list	*next;
	struct s_list	*previous;
}	t_list;

typedef struct s_zone {
	size_t	max_size;
	t_list	*free;
	t_list	*page;
}	t_zone;

typedef union u_memory_pointer {
	void	*as_void;
	size_t	*as_sizeT;
	t_list	*as_Tlist;
	char	*as_char;
}	t_memory_pointer;

// -------------usefull define--------------

//LOG			// for logging the allocation and free
//PRINTF		// for debuging purpose
//MUTEX			// for multithreading
//POISON_FREE	// for poisonning free block
//CHECK_FREE	// for checking if a pointer passed to free or realloc is valide
//COALESCING	// for coalescing
//BEST_FIT		// for changing the finding algorythme

# if BEST_FIT_FT == 1
# pragma message "BEST_FIT is defined"
# endif

# if BEST_FIT_FT == 1
	# define BEST_FIT
# endif

# if MUTEX_FT == 1
# pragma message "MUTEX is defined"
# endif

# if MUTEX_FT == 1
	# define MUTEX
# endif

# if COALESCING_FT == 1
# pragma message "COALESCING is defined"
# endif

# if COALESCING_FT == 1
	# define COALESCING
# endif

# if LOG_FT == 1
# pragma message "LOG_FT is defined"
# endif

# if LOG_FT == 1
	# define LOG
# endif

# if CHECK_FREE_FT == 0
# pragma message "CHECK_FREE is not defined this will fail the test4 but will result in better performance"
# endif

# if CHECK_FREE_FT == 1
	# define CHECK_FREE
# endif

# if POISON_FREE_FT == 1
# pragma message "POISON_FREE is defined"
# endif

# if POISON_FREE_FT == 1
	# define POISON_FREE
# endif

# if PRINTF_FT == 1
# pragma message "PRINTF is defined"
# endif

# if PRINTF_FT == 1
	# define PRINTF
# endif

# if DEBUG_FT == 1
# pragma message "DEBUG is defined"
# endif

# if DEBUG_FT == 1
	#define RED_ZONE_SIZE (2 * sizeof(size_t))
# else
	#define RED_ZONE_SIZE 0
# endif

# if FREE_DELAY_FT != 0
# pragma message "FREE_DELAY is defined"
# endif

# ifndef FREE_DELAY_FT
	# define FREE_DELAY 0
# elif FREE_DELAY_FT < 0
	# define FREE_DELAY 0
# else
	# define FREE_DELAY FREE_DELAY_FT
# endif

# define MAX_SIZET (~(size_t)0)
# define MINIMUM_FREE_BLOCK_SIZE (2 * sizeof(size_t) + sizeof(t_list))
# define MINIMUM_ALLOCATED_BLOCK_SIZE (2 * sizeof(size_t) + 2 * RED_ZONE_SIZE)
# define PAGE_START_OVERHEAD (2 * sizeof(size_t) + sizeof(t_list))
# define PAGE_OVERHEAD (3 * sizeof(size_t) + sizeof(t_list))
# define MINIMUM_PAGE_SIZE (PAGE_OVERHEAD + 2 * RED_ZONE_SIZE)
# define RED_ZONE_COLOR 0xee
# define FREE_COLOR 0xcc
# define unlikely(x)	(__builtin_expect(!!(x), 0))

// -------------global allocation variable-------------

extern t_zone grimoire[11];
extern pthread_mutex_t	mutex; 

// -------------mandatory function---------------

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem();

// -------------bonus function-------------

void    *calloc(size_t nmemb, size_t size);
void	show_alloc_mem_ex();

// -------------memory function----------------

/**
 * @brief simply choose the right grimoir page for the soon to be allocated memory
 * 
 * @param size the size of the requested allocation
 * @return t_zone* the address of the t_zone corresponding to the allocated size
 */
t_zone	*_choose_the_right_page(size_t size);

/**
 * @brief your standard memset function
 * 
 * @param block 
 * @param c 
 * @param size 
 * @return void* 
 */
void	*_ft_memset(void *block, int c, size_t size);

/**
 * @brief return the address of block but red_zonned according to the RED_ZONE_SIZE and RED_ZONE_COLOR variable
 * 
 * @param block the memory block to be redzone
 * @param size_allocated the size of the content of the block to not be redzoned so the padded size
 * @return void* 
 */
void	*_red_zone(void *block, size_t size_allocated);

/**
 * @brief simply set the size first bit at block location to poison 
 * 
 * @param block the memory location to be poison
 * @param size the size of the poison
 * @param poison the poison in char format
 */
void	_poison_block(void *block, size_t size, char poison);

// -------------t_list function----------------

/**
 * @brief the general function to call to add a block to a t_list
 * 
 * @param new_block the block to be added
 * @param list_head the address of the t_list to be modified
 */
void	_add_block_to_t_list(t_list *new_block, t_list **list_head);

/**
 * @brief remove a block from a t_list
 * 
 * @param block the block to be removed from the t_list
 * @param head the address of the list_head to be modified
 */
void	_remove_block_from_t_list(t_list *block, t_list **head);

/**
 * @brief count and display every element in a t_list of free block
 * 
 * @param free_list_head 
 */
void	_printf_free_list(t_list *free_list_head);

// -------------helper function-----------------

/**
 * @brief your standard min function
 * 
 * @param a 
 * @param b 
 * @return size_t the minimum between a and b
 */
size_t	min(size_t a, size_t b);

/**
 * @brief give back a rounded value to the nearest rounding
 * 
 * @param x the value to be rounded
 * @param rounding rounded to rounding
 * @return size_t the rounded value
 */
size_t	_ceilling_unsigned(size_t x, size_t rounding);

/**
 * @brief return the ceilling of size to the nearest size_t
 * 
 * @param size 
 * @return size_t the ceilling to the next size_t multiple
 */
size_t	_padded(size_t size);

// --------------debug function---------------------------

/**
 * @brief a usefull function durring correction to free every allocation
 * 
 */
void	free_everything();

/**
 * @brief a function that return if a pointer is a valide allocated block
 * 
 * @param ptr 
 * @return int 
 */
int 	is_a_valid_address(void *ptr);

/**
 * @brief a debug function ment only for dev
 * 
 * @param memory_location 
 * @param size 
 */
void	_debug_hexa(void *memory_location, size_t size);

#endif