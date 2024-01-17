# include "malloc.h"

void    *calloc(size_t nmemb, size_t size) {
    # ifdef MUTEX
        pthread_mutex_lock(&mutex);
    # endif

    # ifdef LOG
		int fd = open("./log", O_APPEND | O_WRONLY);
		ft_dprintf(fd, "calloc of nmemb : -%d- and size : -%d-\n", nmemb, size);
		close(fd);
    # endif

    if (nmemb == 0 || size == 0) {
        # ifdef LOG
		    int fd = open("./log", O_APPEND | O_WRONLY);
	    	ft_dprintf(fd, "exiting callo returning NULL because : nmemb : -%d- and size : -%d-\n", nmemb, size);
			close(fd);
    	# endif
        # ifdef MUTEX
            pthread_mutex_unlock(&mutex);
        # endif
        return NULL;
    }
    // todo check overflow

    # ifdef MUTEX
        pthread_mutex_unlock(&mutex);
    # endif

    void    *ptr = malloc(nmemb * size);

    # ifdef MUTEX
        pthread_mutex_lock(&mutex);
    # endif

	if (ptr != NULL)
    	ft_memset(ptr, 0, nmemb * size);

    # ifdef LOG
	    fd = open("./log", O_APPEND | O_WRONLY);
	   	ft_dprintf(fd, "exiting calloc returning : -%p-, nmemb : -%d- and size : -%d-\n", ptr, nmemb, size);
		close(fd);
  	# endif

    # ifdef MUTEX
        pthread_mutex_unlock(&mutex);
    # endif

	# ifdef PRINTF
		// show_alloc_mem_ex();
        debug_hexa((void *)((char *)ptr - RED_ZONE_SIZE - sizeof(size_t)), (*(size_t *)((char *)ptr - RED_ZONE_SIZE - sizeof(size_t))) / sizeof(size_t));
	# endif

    return ptr;
}