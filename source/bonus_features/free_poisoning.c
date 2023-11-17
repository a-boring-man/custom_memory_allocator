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