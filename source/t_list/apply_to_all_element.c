#include "malloc.h"
#include <sys/mman.h>

void    remove_page_if(t_list **list_head, int (*condition_function)(void *page), t_zone *zone) {
    if (unlikely(*list_head == NULL)) { // useless protection should never occur but leave it anyway
        return;
    }
    t_list  *ptr = *list_head;

    while (*list_head != ptr->next) { // for all element
        if (condition_function != NULL && condition_function(ptr)) {
            remove_block_from_t_list(ptr, &(zone->page));
            remove_block_from_t_list((t_list *)((char *)ptr + PAGE_START_OVERHEAD), &(zone->free)); // move to the t_list part of the free block
			munmap((size_t *)ptr - 1, *((size_t *)ptr - 1)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value

        }
    }
	if (*list_head == ptr->next) { // for the last element
        if (condition_function != NULL && condition_function(ptr)) {
            remove_block_from_t_list(ptr, &(zone->page));
            remove_block_from_t_list((t_list *)((char *)ptr + PAGE_START_OVERHEAD), &(zone->free)); // move to the t_list part of the free block
			munmap((size_t *)ptr - 1, *((size_t *)ptr - 1)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
        }
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