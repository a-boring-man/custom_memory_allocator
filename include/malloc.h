#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>
#include <unistd.h>

typedef struct s_list {
	void	*next;
	void	*previous;
}	t_list;

typedef struct s_zone {
	size_t	max_size;
	t_list	*free;
	void	*page;
}	t_zone;

static t_zone grimoire[14] __attribute__((unused)) = {
	{8, NULL, NULL},
	{16, NULL, NULL},
	{32, NULL, NULL},
	{64, NULL, NULL},
	{96, NULL, NULL},
	{128, NULL, NULL},
	{192, NULL, NULL},
	{256, NULL, NULL},
	{512, NULL, NULL},
	{1024, NULL, NULL},
	{2048, NULL, NULL},
	{4096, NULL, NULL},
	{8192, NULL, NULL},
	{0, NULL, NULL},
};

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem();
void	show_alloc_mem_ex();

t_zone	*choose_the_right_page(size_t size);
void	*create_page(t_zone *zone, size_t size);
void	*glue_page_together(t_zone *zone, void *new_page, size_t size, size_t page_size);

size_t	ceilling_unsigned(double x);
void	add_block_to_free_list(t_list *new_block, t_list **list_head);

#endif