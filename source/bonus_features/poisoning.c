#include "malloc.h"

void	*ft_memset(void *block, int c, size_t size)
{
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
	ft_memset(block, RED_ZONE_COLOR, RED_ZONE_SIZE);
	ft_memset(block + size_allocated + RED_ZONE_SIZE, RED_ZONE_COLOR, RED_ZONE_SIZE);
	return (block + RED_ZONE_SIZE);
}