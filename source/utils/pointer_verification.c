#include "malloc.h"


int is_a_valid_block(t_list *page, void *ptr) { // check if it's a valide allocated block
    t_memory_pointer    working_pointer;
    working_pointer.as_Tlist = page;

    working_pointer.as_char += (PAGE_START_OVERHEAD - sizeof(size_t)); // move to the first block
    while ((*working_pointer.as_sizeT & -2) != 0) { // not at the end of the page
        if ((void *)(working_pointer.as_char + sizeof(size_t) + RED_ZONE_SIZE) == ptr) {
            return 1;
        }
        working_pointer.as_char += *working_pointer.as_sizeT & -2; // jump to the next block
    }
    return 0;
}

// tell if the ptr is contain within a page
int is_in_the_page(void *ptr, t_list *page) {
    t_memory_pointer    working_pointer;
    working_pointer.as_Tlist = page;

    working_pointer.as_sizeT -= 1; // go to the size of the page
    void *page_start = working_pointer.as_void; // go to the beguinning of the page to get the page size
    working_pointer.as_char += *working_pointer.as_sizeT - sizeof(size_t); // move the pointer to the end of the page
    void *page_end = working_pointer.as_void;
    if (ptr > page_start && ptr < page_end) {
        return 1;
    }
    return 0;
}

int is_a_valid_address(void *ptr) {
    int grimoire_page = 0;
    t_list *page = NULL;

    while (grimoire_page < 11) { // parcour all the grimoir to check if the ptr is on a page
        page = applied_to_all_element(grimoire[grimoire_page].page, is_in_the_page, ptr);
        if (page != NULL) { // a valid page was found
            return (is_a_valid_block(page, ptr));
        }
        grimoire_page++;
    }
    return 0;
}