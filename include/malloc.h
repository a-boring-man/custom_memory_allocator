#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>
#include <unistd.h>

# ifdef RED_ZONE_DEBUG_SIZE
	#if RED_ZONE_DEBUG_SIZE < 16
		#define RED_ZONE_SIZE RED_ZONE_DEBUG_SIZE
	#else
		#define RED_ZONE_SIZE 16
	#endif
# else
	#define RED_ZONE_SIZE 0
# endif

typedef struct s_list {
	struct s_list	*next;
	struct s_list	*previous;
}	t_list;

typedef struct s_zone {
	size_t	max_size;
	t_list	*free;
	t_list	*page;
}	t_zone;

extern t_zone grimoire[11];

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem();
void	show_alloc_mem_ex();

t_zone	*choose_the_right_page(size_t size);
void	*create_page(t_zone *zone, size_t size);

size_t	ceilling_unsigned(double x);
void	add_block_to_t_list_address_ordered(t_list *new_block, t_list **list_head);
void	add_block_to_t_list_first(t_list *new_block, t_list **list_head);
void	poison_block(void *block, size_t size, char poison);

void	format_new_page(void *new_page, size_t page_size);
void	format_free_space(void *new_page, size_t free_block_size);

void	*best_fit(size_t size_to_be_alloc, t_zone *zone);
void	*next_fit(size_t size_to_be_alloc, t_zone *zone);
void	*first_fit(size_t size_to_be_alloc, t_zone *zone);

#endif