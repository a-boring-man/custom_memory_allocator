#include "malloc.h"
#include <sys/mman.h>

void	free_everything() {
	for (int i = 0; i < 11; i++) {
		t_list *current_page = grimoire[i].page;
		while (current_page != NULL) {
			t_list *next = current_page->next;
			remove_block_from_t_list(current_page, &(grimoire[i].page));
			munmap((void *)((size_t *)(current_page) - 1), *((size_t *)(current_page) - 1));
			if (current_page != next)
				current_page = next;
			else
				current_page = grimoire[i].page; // so NULL
		}
		grimoire[i].free = NULL;
	}
}

// will traverse the list of page and applied is in the page if yes it return the page else it return NULL
t_list *applied_to_all_element(t_list *list_head, int (*condition_function)(void *ptr, t_list *page), void *ptr) {
    t_list *current = list_head;
    if (list_head == NULL) { // list is empty
        return NULL;
    }
    while (current->next != list_head) { // all element execpt last
        if (condition_function(ptr, current)) { // if pointer is in page
            return current;
        }
        current = current->next;
    }
    if (condition_function(ptr, current)) { // check for the last element
        return current;
    }
    return NULL;
}

void    remove_all_free_block_inside_page(t_list **free_list, t_list *page) {
	t_memory_pointer	working_pointer;
	working_pointer.as_Tlist = page;
	
	working_pointer.as_char += sizeof(t_list) + sizeof(size_t); // set the pointer to the first block on the page
	while (*working_pointer.as_sizeT != 1) { // go through all the block inside the page until the end
		remove_block_from_t_list((t_list *)(working_pointer.as_sizeT + 1), free_list); // remove the block
		working_pointer.as_char += *working_pointer.as_sizeT;
	}
}

void    remove_page_if(t_list **list_head, int (*condition_function)(void *page), t_zone *zone) {
    t_list  *ptr = *list_head;

    while (ptr != NULL && *list_head != ptr->next) { // for all element execpt the last
        if (condition_function(ptr)) {
			remove_all_free_block_inside_page(&(zone->free), ptr);
            remove_block_from_t_list(ptr, &(zone->page));
			munmap((size_t *)ptr - 1, *((size_t *)ptr - 1)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
            ptr = *list_head;
            continue;
        }
        if (ptr != NULL) // useless but left it anyway
            ptr = ptr->next;
    }
    if (ptr != NULL && condition_function(ptr)) {
		remove_all_free_block_inside_page(&(zone->free), ptr);
        remove_block_from_t_list(ptr, &(zone->page));
	    munmap((size_t *)ptr - 1, *((size_t *)ptr - 1)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
    }
}

void    printf_t_list(t_list *list_head) { // this is a debug function left here on purpose just in case something goes wrong during the defense and may induce some segfault due to debughexa
    t_list *current = list_head;
    size_t element_nbr = 0;

    if (current == NULL) {
        ft_printf("t_list contain %d elements\n", element_nbr);
        return;
    }
    while (current->next != list_head) {
        element_nbr++;
        ft_printf("t_list element nbr : %d is at adress %p\n", element_nbr, current);
        debug_hexa((void *)((size_t *)current -1), 25);
        current = current->next;
    }
    element_nbr++;
    ft_printf("t_list element nbr : %d is at adress %p\n", element_nbr, current);
    debug_hexa((void *)((size_t *)current -1), 25);
    ft_printf("the list contain %d elements\n", element_nbr);
}