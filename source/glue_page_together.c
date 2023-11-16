#include "malloc.h"

void	*glue_page_together(t_zone *zone, void *new_page, size_t size, size_t page_size) {
	(void)size;
	void	*work_pointer = new_page;
	if (zone->page == NULL){
		zone->page = new_page;
		((t_list*)new_page)[0].next = NULL; //set the addres of the next and previous page
		((t_list*)new_page)[0].previous = NULL;
		work_pointer = (char *)work_pointer + sizeof(t_list); // move the pointer past the linked list 
		((size_t *)work_pointer)[0] = page_size - sizeof(t_list); // put the number of free bytes next to the linked list
		work_pointer = (char *)work_pointer + page_size - sizeof(t_list) - sizeof(size_t); // move the pointer to the end of the page
		((size_t *)work_pointer)[0] = page_size - sizeof(t_list); // put the number of free bytes on the last available space in the page

	}
		return NULL;
}