#include "malloc.h"
#include <sys/mman.h>

static void    remove_all_free_block_inside_page(t_list **free_list, t_list *page) {
	t_memory_pointer	working_pointer;
	working_pointer.as_Tlist = page;
	
	working_pointer.as_char += sizeof(t_list) + sizeof(size_t); // set the pointer to the first block on the page
	while (*working_pointer.as_sizeT != 1) { // go through all the block inside the page until the end
		_remove_block_from_t_list((t_list *)(working_pointer.as_sizeT + 1), free_list); // remove the block
		working_pointer.as_char += *working_pointer.as_sizeT;
	}
}

static void    remove_page_if(t_list **list_head, int (*condition_function)(void *page), t_zone *zone) {
    t_list  *ptr = *list_head;

    while (ptr != NULL && *list_head != ptr->next) { // for all element execpt the last
        if (condition_function(ptr)) {
			remove_all_free_block_inside_page(&(zone->free), ptr);
            _remove_block_from_t_list(ptr, &(zone->page));
			munmap((size_t *)ptr - 1, *((size_t *)ptr - 1)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
            ptr = *list_head;
            continue;
        }
        if (ptr != NULL) // useless but left it anyway
            ptr = ptr->next;
    }
    if (ptr != NULL && condition_function(ptr)) {
		remove_all_free_block_inside_page(&(zone->free), ptr);
        _remove_block_from_t_list(ptr, &(zone->page));
	    munmap((size_t *)ptr - 1, *((size_t *)ptr - 1)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
    }
}

# ifdef COALESCING
static	void	coalescing_left(void **block_ptr, t_zone *zone) { // return the left side of a free block coalesced with a potential left free block
	t_memory_pointer	working_pointer;
	working_pointer.as_void = *block_ptr;
	size_t	left_block_size;

	working_pointer.as_sizeT -= 1; // put the pointer to the end of the previous block
	if (*working_pointer.as_sizeT & 1) { // begginning of the page or allocated block
		return ;
	}
	left_block_size = *working_pointer.as_sizeT; // store the original left block size
	_remove_block_from_t_list((t_list *)((size_t *)(*block_ptr) + 1), &(zone->free));
	working_pointer.as_char += *((size_t *)(*block_ptr)); // go to the end of the block to change it's marked size
	*working_pointer.as_sizeT += left_block_size; // change the end block marker
	working_pointer.as_char -= (*working_pointer.as_sizeT - sizeof(size_t)); // go to the first marker to change it
	*working_pointer.as_sizeT += *((size_t *)(*block_ptr)); // change the first marker to the now true size of the block
	working_pointer.as_sizeT += 1; // put the pointer to the t_list part
	_remove_block_from_t_list(working_pointer.as_Tlist, &(zone->free)); // remove the big block from the free list
	_add_block_to_t_list(working_pointer.as_Tlist, &(zone->free)); // re add the block back so it's first on the list to avoid splinter at the beginning of list
	working_pointer.as_sizeT -= 1;
	*block_ptr = working_pointer.as_void; // srt the block address to the very begginning of the coalesced block
}

static	void	coalescing_right(void **block_ptr, t_zone *zone) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = *block_ptr;
	size_t	right_block_size;

	working_pointer.as_char += *((size_t *)(*block_ptr)); // put the pointer to the begginnin of the right block
	if (*working_pointer.as_sizeT & 1) { // if the right block is allocated
		return ;
	}
	right_block_size = *working_pointer.as_sizeT; // store the right part
	working_pointer.as_sizeT += 1; // move to the right block t_list part
	_remove_block_from_t_list(working_pointer.as_Tlist, &(zone->free)); // remove the right free block from the list
	working_pointer.as_sizeT -= 1; // move back to the size of the right block
	working_pointer.as_char += *working_pointer.as_sizeT - sizeof(size_t); // jump to the end
	*working_pointer.as_sizeT += *((size_t *)(*block_ptr)); // add the size of the left block to the end of the right block
	working_pointer.as_char -= (*working_pointer.as_sizeT - sizeof(size_t)); // move to the begginning of the left block
	*working_pointer.as_sizeT += right_block_size; // change it's value to be the sum of the two block lenght
	working_pointer.as_sizeT += 1; // move to the t_list part to change the free list accordingly
	_remove_block_from_t_list(working_pointer.as_Tlist, &(zone->free)); // remove the big block from the free list
	_add_block_to_t_list(working_pointer.as_Tlist, &(zone->free)); // re add the block back so it's first on the list to avoid splinter at the beginning of list
	working_pointer.as_sizeT -= 1;
	*block_ptr = working_pointer.as_void; // srt the block address to the very begginning of the coalesced block
}

static void	coalescing(void *ptr, t_zone *zone) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = ptr;
	
	working_pointer.as_char -= (RED_ZONE_SIZE + sizeof(size_t)); // pointer to the size of the block
	coalescing_left(&working_pointer.as_void, zone);
	coalescing_right(&working_pointer.as_void, zone);
}
# endif

static void	mark_block_as_free(void *block, t_zone *zone) {
	t_memory_pointer	working_pointer;
	
	working_pointer.as_void = block;
	working_pointer.as_char -= (RED_ZONE_SIZE + sizeof(size_t)); // set the address of WP to the beggining of the block
	*working_pointer.as_sizeT = *working_pointer.as_sizeT & -2; // mark as free
	working_pointer.as_char += *working_pointer.as_sizeT - sizeof(size_t); // move to the end of the block
	*working_pointer.as_sizeT = *working_pointer.as_sizeT & -2; // mark as free // here i delibaratly not set the t_list part of the block since i added the setting part to add_block function
	# ifdef POISON_FREE
		_poison_block((char *)block - RED_ZONE_SIZE + sizeof(t_list), *working_pointer.as_sizeT - sizeof(t_list) - 2 * sizeof(size_t), FREE_COLOR);
	# endif
	_add_block_to_t_list((t_list *)((char *)block - RED_ZONE_SIZE), &(zone->free));
}

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
	return (!(*working_pointer.as_sizeT & 1) && (page_size == (*working_pointer.as_sizeT) + PAGE_OVERHEAD || has_only_free_block)); // return true if the first block is not malloc AND either the hol page is a free block or composed of free block only
}

static void check_for_unmap_page(t_zone *zone) {
	(void)zone;
	for (int i = 0; i < 11; i++){
		if (grimoire[i].page == NULL) {
			continue;
		}
		remove_page_if(&(grimoire[i].page), check_if_page_is_empty, &(grimoire[i]));
	}
}

void	free(void *ptr) {
	# ifdef MUTEX
		pthread_mutex_lock(&mutex);
	# endif
	
	# ifdef PRINTF
		// show_alloc_mem_ex();
	# endif
	
	size_t	block_size;
	int		data_size;
	static int free_page_counter = FREE_DELAY; // the number of free before a page cleanup
	
	# ifdef LOG
		int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
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
		
			# ifdef LOG
				int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
				ft_dprintf(fd, "free invalid pointer : -%p-\n", ptr);
				close(fd);
			# endif

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

	t_zone	*zone = _choose_the_right_page(data_size);
	mark_block_as_free(ptr, zone);

	# ifdef COALESCING
		coalescing(ptr, zone);
	# endif

	# ifdef PRINTF
		ft_dprintf(2, "next clean in %d\n", free_page_counter);
	# endif

	if (free_page_counter-- == 0) { // free all empty page once in a while
		check_for_unmap_page(zone);
		free_page_counter = FREE_DELAY;
	}

	# ifdef MUTEX
		pthread_mutex_unlock(&mutex);
	# endif
}