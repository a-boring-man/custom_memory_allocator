#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>
#include <unistd.h>

# ifdef RED_ZONE_DEBUG
	#define RED_ZONE_SIZE 2 * sizeof(size_t)
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

typedef union u_working_pointer {
	void	*m_void;
	size_t	*m_sizet;
	t_list	*m_tlist;
	char	*m_char;
}	t_union;

# define MINIMUM_FREE_BLOCK_SIZE 2 * sizeof(size_t) + sizeof(t_list)
# define MINIMUM_ALLOCATED_BLOCK_SIZE 2 * sizeof(size_t) + 2 * RED_ZONE_SIZE
# define PAGE_OVERHEAD 2 * sizeof(size_t) + sizeof(t_list)
# define MINIMUM_PAGE_SIZE PAGE_OVERHEAD + 2 * RED_ZONE_SIZE
# define RED_ZONE_COLOR 0xCC

extern t_zone grimoire[11];

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem();
void	show_alloc_mem_ex();

t_zone	*choose_the_right_page(size_t size);
void	*create_page(t_zone *zone, size_t size);

size_t	ceilling_unsigned(double x);
void	add_block_to_t_list(t_list *new_block, t_list **list_head);
void	add_block_to_t_list_address_ordered(t_list *new_block, t_list **list_head);
void	add_block_to_t_list_first(t_list *new_block, t_list **list_head);
void	poison_block(void *block, size_t size, char poison);
void	*red_zone(void *block, size_t size_allocated);

void	format_new_page(void *new_page, size_t page_size);
void	format_free_space(void *new_page, size_t free_block_size);
size_t	determine_page_size(t_zone *zone, size_t size_to_be_malloc);
void	remove_block_from_t_list(t_list *block, t_list **head);
void	*mark_block_as_allocated(t_list *block, size_t size_to_be_allocated, t_zone * zone);

void	*best_fit(size_t size_to_be_alloc, t_zone *zone);
void	*next_fit(size_t size_to_be_alloc, t_zone *zone);
void	*first_fit(size_t size_to_be_alloc, t_zone *zone);

#endif