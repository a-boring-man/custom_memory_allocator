#include "malloc.h"
#include <sys/mman.h>

void    remove_page_if(t_list **list_head, int (*condition_function)(void *page)) {
    if (unlikely(*list_head == NULL)) { // useless protection should never occur but leave it anyway
        return;
    }
    t_list  *ptr = *list_head;

    while (*list_head != ptr->next) { // for all element
        if (condition_function != NULL && condition_function(ptr)) {
            remove_block_from_t_list(ptr, list_head);
			munmap((size_t *)ptr - 1, *((size_t *)ptr - 1)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
        }
    }
	if (*list_head == ptr->next) { // for the last element
        if (condition_function != NULL && condition_function(ptr)) {
            remove_block_from_t_list(ptr, list_head);
			munmap((size_t *)ptr - 1, *((size_t *)ptr - 1)); // cannot do anything if munmap return -1 because the programe will deallocated it's memory anyway and free cannot return a value
        }
	}
}