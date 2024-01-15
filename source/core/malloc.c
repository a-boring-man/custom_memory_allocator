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
	return_ptr = find_free_block(size, zone);
	
	# ifdef LOG	
		int fd = open("./log", O_APPEND | O_WRONLY);
		ft_dprintf(fd, "malloc : -%p- size : -%d-\n", return_ptr, size);
		close(fd);
	# endif
	
	# ifdef MUTEX
		pthread_mutex_unlock(&mutex);
	# endif

	# ifdef PRINTF
		
	# endif
	
	return (return_ptr);
}