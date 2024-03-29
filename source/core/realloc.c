#include "malloc.h"
//								FREE BLOCK BEFORE
////////////////////////////////////////////////////////////////////////////
//S(S | T |                                                          |S(S //
//I I | _ |                                                          |I I //
//Z Z | L |                                                          |Z Z //
//E E | I |                                                          |E E //
//  _ | S |                                                          |  _ //
//  T)| T |                                                          |  T)//
////////////////////////////////////////////////////////////////////////////
//									|
//									|
//									|
//									|
//									|
//									V
//								SPLITED_BLOCK
////////////////////////////////////////////////////////////////////////////
//S(S |R Z|                              |R Z|S(S |S(S | T |         |S(S //
//I I |E O|                              |E O|I I |I I | _ |         |I I //
//Z Z |D N|	P A Y L O A D                |D N|Z Z |Z Z | L |FREE ZONE|Z Z //
//E E |  E|                              |  E|E E |E E | I |         |E E //
//  _ |   |                              |   |  _ |  _ | S |         |  _ //
//  T)|   |                              |   |  T)|  T)| T |         |  T)//
////////////////////////////////////////////////////////////////////////////
//									|
//									OR
//									|
//									V
//								WASTED_BLOCK
////////////////////////////////////////////////////////////////////////////
//S(S |R Z|                                                     |R Z|S(S |//
//I I |E O|                                                     |E O|I I |//
//Z Z |D N|	P A Y L O A D                                       |D N|Z Z |//
//E E |  E|                                                     |  E|E E |//
//  _ |   |                                                     |   |  _ |//
//  T)|   |                                                     |   |  T)|//
////////////////////////////////////////////////////////////////////////////


static void	mark_block_as_allocated_from_realloc(void *block_beginning, t_zone *zone, size_t size) { // here size is the new size requested by realloc padded. here i already remove the free block from the list
	t_memory_pointer	working_pointer;
	working_pointer.as_void = block_beginning;
	size_t	block_size = *working_pointer.as_sizeT & -2; // store the block size for later use
	size_t	needed_size = size + MINIMUM_ALLOCATED_BLOCK_SIZE;
	int		should_be_split = block_size - needed_size >= MINIMUM_FREE_BLOCK_SIZE; // determini if the block shoul be split into a allocated block and a free block

	if (should_be_split) {
		size_t	left_over = block_size - needed_size;
		*working_pointer.as_sizeT = needed_size + 1; // put the true needed size
		working_pointer.as_char += (needed_size - sizeof(size_t)); // move the pointer to the end of the nwely marked allocated block
		*working_pointer.as_sizeT = needed_size + 1; // mark the end of the block
		working_pointer.as_sizeT += 1; // move to the free section
		*working_pointer.as_sizeT = left_over; // write the size of the free block
		working_pointer.as_char += (left_over - sizeof(size_t)); // go to the end of the free block
		*working_pointer.as_sizeT = left_over; // store the end marker
		working_pointer.as_char -= (*working_pointer.as_sizeT - 2 * sizeof(size_t)); // return at the t_list emplacement
		_add_block_to_t_list(working_pointer.as_Tlist, &(zone->free)); // add the last block to the free list
		_red_zone((char *)block_beginning + sizeof(size_t), size); // recollor the block
	}
	else { // cant split it
		*working_pointer.as_sizeT = block_size + 1; // mark bothe end of the block
		working_pointer.as_char += (block_size - sizeof(size_t));
		*working_pointer.as_sizeT = block_size + 1;
		working_pointer.as_char -= (block_size - 2 * sizeof(size_t)); // go back to the t_list part of the allocated block
		_red_zone(working_pointer.as_Tlist, size);
	}
}

static void	*ft_memcpy(void *to, void *from, size_t size) {
	size_t	i;

	i = 0;
	if (!to && ! from)
		return (NULL);
	while (i < size)
	{
		*(char *)(to + i) = *(char *)(from + i);
		i++;
	}
	return (to);
}

void	*realloc(void *ptr, size_t size) {
	# ifdef MUTEX
		pthread_mutex_lock(&mutex);
	# endif
	# ifdef PRINTF
		ft_dprintf(2, "in realloc pointer is : -%p- and size : -%d-\n", ptr, size);
	# endif

	if (size < 16) {
		size = 16;
	}
	# ifdef LOG
		int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
		ft_dprintf(fd, "realloc enter : -%p- with size : -%d-\n", ptr, size);
		close(fd);
	# endif

	if (ptr == NULL) { // if ptr is NULL the call is equivalent to malloc(size) regardless of size
		# ifdef LOG
			int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
			ft_dprintf(fd, "realloc exit because of null pointer calling malloc : -%p-\n", ptr);
			close(fd);
		# endif
		# ifdef MUTEX
			pthread_mutex_unlock(&mutex);
		# endif
		return (malloc(size));
	}

	# ifdef CHECK_FREE
		if (!is_a_valid_address(ptr)) {
			# ifdef LOG
				int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
				ft_dprintf(fd, "realloc receive un invalide pointer, returning NULL : -%p-\n", ptr);
				close(fd);
			# endif
			# ifdef MUTEX
				pthread_mutex_unlock(&mutex);
			# endif
			return (NULL);
		}
	# endif

	if (size == 0) { // if size == 0 the call is equivalent to free(ptr) and NULL can be returned
		# ifdef LOG
			int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
			ft_dprintf(fd, "realloc exit because of size 0 calling free : -%p-\n", ptr);
			close(fd);
		# endif
		# ifdef MUTEX
			pthread_mutex_unlock(&mutex);
		# endif
		free(ptr);
		return NULL;
	}

	t_memory_pointer	working_pointer;
	working_pointer.as_void = ptr;
	working_pointer.as_char -= (sizeof(size_t) + RED_ZONE_SIZE); // put the working_pointer to the size of the block
	
	size_t	left_block_size = *working_pointer.as_sizeT & -2;
	size_t	data_size = left_block_size - MINIMUM_ALLOCATED_BLOCK_SIZE;
	if (_padded(size) == data_size) {
		# ifdef LOG
			int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
			ft_dprintf(fd, "realloc exit : -%p-\n", ptr);
			close(fd);
		# endif

		# ifdef MUTEX
			pthread_mutex_unlock(&mutex);
		# endif

		return ptr;
	}

	t_zone	*block_zone = _choose_the_right_page(data_size); // get the zone of the data to be realloc
	t_zone	*new_block_zone = _choose_the_right_page(size); // get the zone of the newly size malloc
	int need_to_be_moved = block_zone != new_block_zone; // check to see if the content need to be moved

	if (need_to_be_moved || size > data_size) { // need to check if a free space is next to the block and big enought else send back to malloc
		
		# ifdef PRINTF
			ft_dprintf(2, "need to be moved or greater size\n");
		# endif

		working_pointer.as_char += left_block_size; // move the pointer to the next block to check if it's free
		if (!(*working_pointer.as_sizeT & 1) && *working_pointer.as_sizeT >= (_padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE) && !need_to_be_moved) { // if block is free and big enough and data doesn't need to be moved
			size_t	right_block_size = *working_pointer.as_sizeT; // store the right block size
			_remove_block_from_t_list((t_list *)(working_pointer.as_sizeT + 1), &(block_zone->free)); // remove the right free block from the list
			working_pointer.as_char += (right_block_size - sizeof(size_t)); // go to the end of the right block
			*working_pointer.as_sizeT = right_block_size + left_block_size; // put the lenght of the two block and mark it as free;
			working_pointer.as_char -= (*working_pointer.as_sizeT - sizeof(size_t)); // go back to the beginning
			*working_pointer.as_sizeT = right_block_size + left_block_size; // set the left size to the size of the two block as free block
			mark_block_as_allocated_from_realloc(working_pointer.as_void, block_zone, _padded(size));

			# ifdef LOG
				int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
				ft_dprintf(fd, "realloc exit : -%p-\n", ptr);
				close(fd);
			# endif

			# ifdef MUTEX
				pthread_mutex_unlock(&mutex);
			# endif

			return (ptr);
		}
		else { // call malloc then copy if need to be moved
		 
			# ifdef PRINTF
				ft_dprintf(2, "need to be moved\n");
			# endif
			
			# ifdef MUTEX
				pthread_mutex_unlock(&mutex);
			# endif
			
			void	*new_pointer = malloc(size);
			
			# ifdef MUTEX
				pthread_mutex_lock(&mutex);
			# endif
			
			if (new_pointer != NULL)
				ft_memcpy(new_pointer, ptr, min(data_size, _padded(size))); // copy the old content
			
			# ifdef LOG
				int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
				ft_dprintf(fd, "realloc exit calling free: -%p-\n", new_pointer);
				close(fd);
			# endif

			# ifdef MUTEX
				pthread_mutex_unlock(&mutex);
			# endif

			free(ptr);
			return new_pointer;
		}
	}
	else { // the size will be shrinked
		
		# ifdef PRINTF
			ft_dprintf(2, "doesn't need to be moved\n");
		# endif
		
		int should_be_split = data_size - _padded(size) >= MINIMUM_FREE_BLOCK_SIZE; // if there is enough space to put a new free_block
		size_t	left_over = data_size - _padded(size);
		working_pointer.as_char += left_block_size; // move the wp to the next block to see if it's a free block
		int	has_a_free_block_behind = !(*working_pointer.as_sizeT & 1); // self explenatory
		size_t	free_block_size = 0;

		if (has_a_free_block_behind) {// free block behind can be split and merge to the free block
			free_block_size = *working_pointer.as_sizeT & -2; // store the size of the free block
			_remove_block_from_t_list((t_list *)(working_pointer.as_sizeT + 1), &(block_zone->free)); // remove it from the list
			working_pointer.as_char -= (left_over); // go back to the now new beguinning of the freeblock
			*working_pointer.as_sizeT = free_block_size + left_over; // put the now bigger size in it
			working_pointer.as_char += *working_pointer.as_sizeT - sizeof(size_t); // move to the end of the free block
			*working_pointer.as_sizeT = free_block_size + left_over; // modify the size of the block
			working_pointer.as_char -= (*working_pointer.as_sizeT - 2 * sizeof(size_t)); // go back to the t_list part
			_add_block_to_t_list(working_pointer.as_Tlist, &(block_zone->free)); // readd the block to the list of free block
			working_pointer.as_sizeT -= 2; // move to the now end of the malloced block
			*working_pointer.as_sizeT = _padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE + 1; // write the new block size
			working_pointer.as_char -= (_padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE - sizeof(size_t)); // go to the begginning of the block
			*working_pointer.as_sizeT = _padded(size) + MINIMUM_ALLOCATED_BLOCK_SIZE + 1; // write the new block size
			void *return_pointer = _red_zone((void *)(working_pointer.as_char + sizeof(size_t)), _padded(size));
			
			# ifdef LOG
				int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
				ft_dprintf(fd, "realloc exit : -%p-\n", return_pointer);
				close(fd);
			# endif

			# ifdef MUTEX
				pthread_mutex_unlock(&mutex);
			# endif

			return (return_pointer); // re redzone the block
		}
		else if (should_be_split) {//can be split
			working_pointer.as_sizeT -= 1; // move back to the end of the allocated block
			*working_pointer.as_sizeT = left_over; // put the new free block size size
			working_pointer.as_char -= (left_over - sizeof(size_t)); // go back to the now begginning of the free block
			*working_pointer.as_sizeT = left_over; // put the size of the new freeblock
			_add_block_to_t_list((t_list *)(working_pointer.as_sizeT + 1), &(block_zone->free));
			working_pointer.as_sizeT -= 1; // move to the now end of the allocated block
			*working_pointer.as_sizeT = left_block_size - left_over + 1; // put the new size
			working_pointer.as_char -= ((*working_pointer.as_sizeT & -2) - sizeof(size_t)); // move back to the beginning
			*working_pointer.as_sizeT = left_block_size - left_over + 1; // put the new size
			void *return_pointer = _red_zone((void *)(working_pointer.as_sizeT + 1), _padded(size));
			
			# ifdef LOG
				int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
				ft_dprintf(fd, "realloc exit : -%p-\n", return_pointer);
				close(fd);
			# endif

			# ifdef MUTEX
				pthread_mutex_unlock(&mutex);
			# endif

			return (return_pointer);
		}
		else {// block should be shrinked but no free block to the right and not shrink enought to allow a free block to appear

			# ifdef LOG
				int fd = open("./log", O_APPEND | O_WRONLY | O_CREAT);
				ft_dprintf(fd, "realloc exit : -%p-\n", ptr);
				close(fd);
			# endif

			# ifdef MUTEX
				pthread_mutex_unlock(&mutex);
			# endif

			return (ptr);
		}
	}
}