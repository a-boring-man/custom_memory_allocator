# include "malloc.h"

size_t	max(size_t a, size_t b) {
	if (a >= b) {
		return a;
	}
	return b;
}

static int	will_overflow(size_t a, size_t b) {
	size_t lmax = max(a, b);
	size_t max_min_value = MAX_SIZET / lmax; // check the maximum value of the minimum between a and b for the multiplication between the two to not overflow
	return (min(a, b) > max_min_value); // return if the minimum between a and b is greater than the maximum value it can be without causing an overflow during multiplication
}

void    *calloc(size_t nmemb, size_t size) {
    # ifdef MUTEX
        pthread_mutex_lock(&mutex);
    # endif

    # ifdef LOG
		int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
		ft_dprintf(fd, "calloc of nmemb : -%d- and size : -%d-\n", nmemb, size);
		close(fd);
    # endif

    if (nmemb == 0 || size == 0) {
        # ifdef LOG
		    int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
	    	ft_dprintf(fd, "exiting callo returning NULL because : nmemb : -%d- and size : -%d-\n", nmemb, size);
			close(fd);
    	# endif
        # ifdef MUTEX
            pthread_mutex_unlock(&mutex);
        # endif
        return NULL;
    }
    if (will_overflow(nmemb, size)) {
        # ifdef LOG
		    int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
	    	ft_dprintf(fd, "exiting callo returning NULL because of potential overflow : nmemb : -%d- and size : -%d-\n", nmemb, size);
			close(fd);
    	# endif
        # ifdef MUTEX
            pthread_mutex_unlock(&mutex);
        # endif
        return NULL;
    }

    # ifdef MUTEX
        pthread_mutex_unlock(&mutex);
    # endif

    void    *ptr = malloc(nmemb * size);

    # ifdef MUTEX
        pthread_mutex_lock(&mutex);
    # endif

	if (ptr != NULL)
    	_ft_memset(ptr, 0, nmemb * size);

    # ifdef LOG
	    fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
	   	ft_dprintf(fd, "exiting calloc returning : -%p-, nmemb : -%d- and size : -%d-\n", ptr, nmemb, size);
		close(fd);
  	# endif

	# ifdef PRINTF
		// show_alloc_mem_ex();
        //debug_hexa((void *)((char *)ptr - RED_ZONE_SIZE - sizeof(size_t)), (*(size_t *)((char *)ptr - RED_ZONE_SIZE - sizeof(size_t))) / sizeof(size_t));
	# endif

    # ifdef MUTEX
        pthread_mutex_unlock(&mutex);
    # endif

    return ptr;
}