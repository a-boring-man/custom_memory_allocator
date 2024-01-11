#include "malloc.h"
#include <sys/mman.h>

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

void    remove_page_if(t_list **list_head, int (*condition_function)(void *page), t_zone *zone) {
    t_list  *ptr = *list_head;

    while (ptr != NULL && *list_head != ptr->next) { // for all element execpt the last
        if (condition_function(ptr)) {
            remove_block_from_t_list((t_list *)((char *)ptr + PAGE_START_OVERHEAD), &(zone->free)); // move to the t_list part of the free block
            remove_block_from_t_list(ptr, &(zone->page));
			munmap((size_t *)ptr - 1, *((size_t *)ptr)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
            ptr = *list_head;
            continue;
        }
        if (ptr != NULL)
            ptr = ptr->next;
    }
    if (ptr != NULL && condition_function(ptr)) {
        remove_block_from_t_list(ptr, &(zone->page));
        remove_block_from_t_list((t_list *)((char *)ptr + PAGE_START_OVERHEAD), &(zone->free)); // move to the t_list part of the free block
	    munmap((size_t *)ptr - 1, *((size_t *)ptr)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
    }
}

void    printf_t_list(t_list *list_head) {
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