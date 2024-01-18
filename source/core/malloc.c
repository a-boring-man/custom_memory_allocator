#include "malloc.h"
#include <sys/mman.h>

static size_t	Ceil_to_pagesize_integer(size_t size) {
	size_t	computer_page_size = getpagesize();

	return (ceilling_unsigned(size, computer_page_size) * computer_page_size);
}

static size_t	determine_page_size(t_zone *zone, size_t size_to_be_malloc) {
	size_t	page_size;

	if (zone->max_size == 0) {
		page_size = padded(size_to_be_malloc) + PAGE_OVERHEAD + MINIMUM_ALLOCATED_BLOCK_SIZE;
	}
	else if (RED_ZONE_SIZE != 0 && zone->max_size <= 256) {
		page_size = zone->max_size * 256;
	}
	else {
		page_size = zone->max_size * 128;
	}
	return Ceil_to_pagesize_integer(page_size);
}

//								  PAGE
////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
//									|
//									|
//									|
//									|
//									|
//									V
//								  PAGE
////////////////////////////////////////////////////////////////////////////
//S(S | T | 1(S |                                                    |1(S //
//I I | _ |   I |                                                    |  I //
//Z Z | L |   Z |    F   R   E   E       B   L   O   C   K           |  Z //
//E E | I |   E |                                                    |  E //
//  _ | S |   _ |                                                    |  _ //
//  T)| T |   T)|                                                    |  T)//
////////////////////////////////////////////////////////////////////////////


// 								FREE BLOCK
////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
//									|
//									|
//									|
//									|
//									|
//									V
//								FREE BLOCK
////////////////////////////////////////////////////////////////////////////
//S(S | T |                                                          |S(S //
//I I | _ |                                                          |I I //
//Z Z | L |                                                          |Z Z //
//E E | I |                                                          |E E //
//  _ | S |                                                          |  _ //
//  T)| T |                                                          |  T)//
////////////////////////////////////////////////////////////////////////////

static void	format_free_space(void *new_page, size_t free_block_size) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = new_page;
	
	*(working_pointer.as_sizeT) = free_block_size;
	working_pointer.as_sizeT += 1;
	(working_pointer.as_Tlist)->next = working_pointer.as_Tlist;
	(working_pointer.as_Tlist)->previous = working_pointer.as_Tlist;
	working_pointer.as_char += free_block_size - 2 * sizeof(size_t); // move to the end of the free block
	*(working_pointer.as_sizeT) = free_block_size;
	
	# ifdef POISON_FREE
		poison_block((void *)(working_pointer.as_char - free_block_size + 2 * sizeof(size_t) + sizeof(t_list)), free_block_size - (MINIMUM_FREE_BLOCK_SIZE), FREE_COLOR);
	# endif
} 

static void	format_new_page(void *new_page, size_t page_size) {
	t_memory_pointer	working_pointer;
	working_pointer.as_void = new_page;
	
	*(working_pointer.as_sizeT) = page_size;
	working_pointer.as_sizeT += 1;
	(working_pointer.as_Tlist)->next = working_pointer.as_Tlist;
	(working_pointer.as_Tlist)->previous = working_pointer.as_Tlist;
	working_pointer.as_Tlist += 1;
	*(working_pointer.as_sizeT) = 1;
	working_pointer.as_char += page_size - 2 * sizeof(size_t) - sizeof(t_list);
	*(working_pointer.as_sizeT) = 1;
	format_free_space((char *)new_page + PAGE_START_OVERHEAD, page_size - (PAGE_OVERHEAD));
}

static void	*create_page(t_zone *zone, size_t size) {
	void	*new_page = NULL;

	if (zone->max_size == 0) {
		new_page = mmap(0, padded(size) + PAGE_OVERHEAD + MINIMUM_ALLOCATED_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	}
	else if (RED_ZONE_SIZE != 0 && zone->max_size <= 256) {
		new_page = mmap(0, zone->max_size * 256, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	}
	else {
		new_page = mmap(0, zone->max_size * 128, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	}
	return new_page;
}

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

static void	*mark_block_as_allocated(t_list *block, size_t size_to_be_allocated, t_zone *zone) { // should be use with the padded allocated size
	t_memory_pointer	working_pointer;
	working_pointer.as_Tlist = block;
	
	working_pointer.as_sizeT -= 1; // go back to the beginning of the block
	size_t	block_size = (*working_pointer.as_sizeT & -2); //  get the block size
	size_t	left_over = block_size - (size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE); //  calculate the left over
	
	if (left_over >= MINIMUM_FREE_BLOCK_SIZE) { // if the block can be split in a allocated block + a free block

		remove_block_from_t_list(block, &(zone->free)); // remove the block from the free list
		*working_pointer.as_sizeT = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1;
		working_pointer.as_char += size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE - sizeof(size_t);
		*working_pointer.as_sizeT = size_to_be_allocated + MINIMUM_ALLOCATED_BLOCK_SIZE + 1;
		working_pointer.as_sizeT += 1; // move to the new free block location
		*working_pointer.as_sizeT = left_over;
		working_pointer.as_sizeT += 1; // move to the t_list part
		working_pointer.as_Tlist->next = working_pointer.as_Tlist;
		working_pointer.as_Tlist->previous = working_pointer.as_Tlist;
		add_block_to_t_list(working_pointer.as_Tlist, &(zone->free));
		working_pointer.as_char += left_over - 2 * sizeof(size_t); // move to the end
		*working_pointer.as_sizeT = left_over;
		return red_zone(block, size_to_be_allocated);
	}
	else { // if it can only contain the payload
		remove_block_from_t_list(block, &(zone->free));
		*(working_pointer.as_sizeT) += 1;
		working_pointer.as_char += *(working_pointer.as_sizeT) - 1 - sizeof(size_t);
		*(working_pointer.as_sizeT) += 1;
		return red_zone(block, size_to_be_allocated);
	}
}

static void	*best_fit(size_t size_to_be_alloc, t_zone *zone) { // size if the raw size
	t_list	*list_head = zone->free;
	size_t	left_over = MAX_SIZET;
	t_list	*best_block = NULL;
	size_t	true_size = padded(size_to_be_alloc) + MINIMUM_ALLOCATED_BLOCK_SIZE;

	// whyle free list is not empty and free block size <= size_to_be_allocated continue
	while (list_head != NULL \
		&& list_head->next != zone->free \
		&& left_over != 0)
	{
		if (*((size_t *)list_head - 1) >= true_size && left_over > (*((size_t *)list_head -1) - true_size)) {
			best_block = list_head;
			left_over = *((size_t *)list_head -1) - true_size;
		}
		list_head = list_head->next;
	}
	if (list_head == NULL || (*((size_t *)list_head - 1) < true_size && best_block == NULL)) { // no block was found need to create a new page
		t_memory_pointer	new_page;

		new_page.as_void = create_page(zone, size_to_be_alloc); 
		if (new_page.as_void == (void *)-1)
			return NULL; 
		format_new_page(new_page.as_void, determine_page_size(zone, size_to_be_alloc));
		add_block_to_t_list((t_list *)(new_page.as_sizeT + 1), (&(zone->page)));
		add_block_to_t_list((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), &(zone->free));
		return (mark_block_as_allocated((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), padded(size_to_be_alloc), zone));
	}
	else if (*((size_t *)list_head - 1) >= true_size && left_over > (*((size_t *)list_head -1) - true_size)) { // lats member is the best
		return (mark_block_as_allocated(list_head, padded(size_to_be_alloc), zone));
	}
	else { // a best block was found in the list
		return (mark_block_as_allocated(best_block, padded(size_to_be_alloc), zone));
	}
}

static void	*first_fit(size_t size_to_be_alloc, t_zone *zone) {
	t_list	*list_head = zone->free;

	if (size_to_be_alloc < 16) { // useless security
		size_to_be_alloc = 16;
	}
	// while free list is not empty and free block size <= size_to_be_allocated continue
	while (list_head != NULL \
		&& list_head->next != zone->free \
		&& *((size_t *)(list_head) - 1) < padded(size_to_be_alloc) + MINIMUM_ALLOCATED_BLOCK_SIZE)
	{
		list_head = list_head->next;
	}
	if (list_head == NULL || (list_head->next == zone->free && *((size_t *)(list_head) - 1) < padded(size_to_be_alloc) + MINIMUM_ALLOCATED_BLOCK_SIZE)) { // no block was found need to create a new page
		# ifdef PRINTF
			ft_dprintf(2, "must allocate a new page\n");
		# endif
	
		t_memory_pointer	new_page;

		new_page.as_void = create_page(zone, size_to_be_alloc);
		if (new_page.as_void == (void *)-1)
			return NULL; 
		format_new_page(new_page.as_void, determine_page_size(zone, size_to_be_alloc));
		add_block_to_t_list((t_list *)(new_page.as_sizeT + 1), &(zone->page));
		add_block_to_t_list((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), &(zone->free));
		return (mark_block_as_allocated((t_list *)(new_page.as_char + PAGE_START_OVERHEAD + sizeof(size_t)), padded(size_to_be_alloc), zone));
	}
	else { // if a block was found
		return (mark_block_as_allocated(list_head, padded(size_to_be_alloc), zone));
	}
}

static void	*find_free_block(size_t size_to_be_alloc, t_zone *zone) { // select the correct algorithme
	# ifdef BEST_FIT
		return(best_fit(size_to_be_alloc, zone));
	# endif

	return(first_fit(size_to_be_alloc, zone));
}

void	*malloc(size_t size) {
	# ifdef MUTEX
		pthread_mutex_lock(&mutex);
	# endif

	# ifdef PRINTF
		ft_dprintf(2, "my_malloc %u\n", size);
	# endif

	if (size == 0) {
		# ifdef MUTEX
			pthread_mutex_unlock(&mutex);
		# endif
		return NULL;
	}

	void	*return_ptr;

	t_zone	*zone = choose_the_right_page(size);

	if (size < 16) {
		size = 16;
	}

	return_ptr = find_free_block(size, zone);
	
	# ifdef LOG	
		int fd = open("./log", O_APPEND | O_WRONLY);
		ft_dprintf(fd, "malloc : -%p- size : -%d-\n", return_ptr, size);
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