#include "malloc.h"

void	*ft_memset(void *block, int c, size_t size) {
	size_t	i;

	i = 0;
	while (i < size)
	{
		*(unsigned char *)(block + i) = (unsigned char)c;
		i++;
	}
	return (block);
}

void	poison_block(void *block, size_t size, char poison) {
	ft_memset(block, poison, size);
}

void	*red_zone(void *block, size_t size_allocated) {
	if (RED_ZONE_SIZE == 0)
		return block;

	poison_block(block, RED_ZONE_SIZE, RED_ZONE_COLOR);
	poison_block((void *)((char *)block + size_allocated + RED_ZONE_SIZE), RED_ZONE_SIZE, RED_ZONE_COLOR);

	return ((void *)((char *)block + RED_ZONE_SIZE));
}