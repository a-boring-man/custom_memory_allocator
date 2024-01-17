#include "malloc.h"

void	*malloc(size_t size) {
	# ifdef MUTEX
		pthread_mutex_lock(&mutex);
	# endif

	void	*return_ptr;

	if (size == 0) {
		# ifdef MUTEX
			pthread_mutex_unlock(&mutex);
		# endif
		return NULL;
	}

	# ifdef PRINTF
		ft_dprintf(2, "my_malloc %u\n", size);
	# endif
	
	t_zone	*zone = choose_the_right_page(size);
	if (size < 16) {
		size = 16;
	}
	return_ptr = find_free_block(size, zone);
	
	# ifdef LOG	
		int fd = open("./log", O_APPEND | O_WRONLY);
		ft_dprintf(fd, "malloc : -%p- size : -%d-\n", return_ptr, size);
		if (size > 9000) {
			debug_hexa((char *)return_ptr - RED_ZONE_SIZE - sizeof(size_t), 1);
			debug_hexa((char *)return_ptr - RED_ZONE_SIZE - 2 * sizeof(size_t) + *(size_t *)((char *)return_ptr - RED_ZONE_SIZE - sizeof(size_t)), 1);
		}
		close(fd);
	# endif
	
	# ifdef PRINTF
		ft_dprintf(2, "calling debug with ptr : %p and size %d : \n", (char *)return_ptr - RED_ZONE_SIZE - sizeof(size_t), padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE);
		debug_hexa((char *)return_ptr - RED_ZONE_SIZE - sizeof(size_t), (padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE) / sizeof(size_t));
	# endif

	# ifdef MUTEX
		pthread_mutex_unlock(&mutex);
	# endif
	
	return (return_ptr);
}