#include "malloc.h"

void	*_ft_memset(void *block, int c, size_t size) {
	size_t	i;

	i = 0;
	while (i < size)
	{
		*(unsigned char *)(block + i) = (unsigned char)c;
		i++;
	}
	return (block);
}

void	_poison_block(void *block, size_t size, char poison) {
	_ft_memset(block, poison, size);
}

void	*_red_zone(void *block, size_t size_allocated) {
	if (RED_ZONE_SIZE == 0)
		return block;

	_poison_block(block, RED_ZONE_SIZE, RED_ZONE_COLOR);
	_poison_block((void *)((char *)block + size_allocated + RED_ZONE_SIZE), RED_ZONE_SIZE, RED_ZONE_COLOR);

	return ((void *)((char *)block + RED_ZONE_SIZE));
}