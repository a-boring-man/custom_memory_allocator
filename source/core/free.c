#include "malloc.h"

static size_t check_if_only_contain_free_block(void *first_free_block) { // the addres is the begginning of the size of the freeblock
	t_memory_pointer	working_pointer;
	working_pointer.as_void = first_free_block;
	
	while (!(*working_pointer.as_sizeT & 1)) { // stop if a block is allocated
		working_pointer.as_char += *working_pointer.as_sizeT; // jump to the next block
	}
	return (*working_pointer.as_sizeT == 1); // return true if we stop because we reach the marker of end of page
}

static int check_if_page_is_empty(void *page) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = page;
	
	size_t	page_size;
	size_t	has_only_free_block = 0;

	working_pointer.as_sizeT -= 1; // move the pointer to the beginning of the page
	page_size = *working_pointer.as_sizeT; // store page size
	working_pointer.as_char += PAGE_START_OVERHEAD;
	has_only_free_block = check_if_only_contain_free_block(working_pointer.as_void);
	return (!(*working_pointer.as_sizeT & 1) && (page_size == (*working_pointer.as_sizeT & -2) + PAGE_OVERHEAD || has_only_free_block)); // return true if the first block is not malloc AND either the hol page is a free block or composed of free block only
}

static void check_for_unmap_page(t_zone *zone) {
	for (int i = 0; i < 11; i++){
		if (grimoire[i].page == NULL) {
			continue;
		}
		remove_page_if(&(grimoire[i].page), check_if_page_is_empty, zone);
	}
}

void	free(void *ptr) {
	# ifdef MUTEX
		pthread_mutex_lock(&mutex);
	# endif
	
	size_t	block_size;
	int		data_size;
	static int free_page_counter = FREE_DELAY; // the number of free before a page cleanup
	
	# ifdef LOG
		int fd = open("./log", O_APPEND | O_WRONLY);
		ft_dprintf(fd, "free : -%p-\n", ptr);
		close(fd);
	# endif
	
	if (ptr == NULL) {
		# ifdef MUTEX
			pthread_mutex_unlock(&mutex);
		# endif
		return;
	}

	# ifdef CHECK_FREE
		if (!is_a_valid_address(ptr)) { // check if the pointer is a valid pointer
			# ifdef PRINTF
				ft_dprintf(2, "wanting to free a pointer that is not valid\n");
			# endif
	
			# ifdef MUTEX
				pthread_mutex_unlock(&mutex);
			# endif
			return;
		}
	# endif

	block_size = *(size_t *)((char *)ptr - (sizeof(size_t) + RED_ZONE_SIZE)) & -2;
	data_size = block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;

	# ifdef PRINTF
		ft_dprintf(2, "wanting to free the block at : -%p- of size : -%u- and of true size : -%u-\n", ptr, block_size, data_size);
	# endif

	t_zone	*zone = choose_the_right_page(data_size);
	mark_block_as_free(ptr, zone);

	# ifdef COALESCING
		coalescing(ptr, zone);
	# endif

	if (free_page_counter-- == 0) { // free all empty page once in a while
		check_for_unmap_page(zone);
		free_page_counter = FREE_DELAY;
	}

	# ifdef MUTEX
		pthread_mutex_unlock(&mutex);
	# endif
}