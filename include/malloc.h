#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>
#include <stdio.h>

void	my_free(void *ptr);
void	*my_malloc(size_t size);
void	*my_realloc(void *ptr, size_t size);
void	show_alloc_mem();
void	show_alloc_mem_ex();

#endif