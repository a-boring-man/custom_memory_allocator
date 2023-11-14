#include "malloc.h"

void	*glue_page_together(t_zone *zone, void *new_page, size_t size, size_t page_size) {
	(void)size;
	void	*work_pointer = new_page;
	if (zone->page == NULL){
		zone->page = new_page;
		((t_list*)new_page)[0].next = NULL;
		((t_list*)new_page)[0].previous = NULL;
		work_pointer = (char *)work_pointer + sizeof(t_list); 
		((size_t *)work_pointer)[0] = ceiling(((zone->max_size * 128) / getpagesize())) * getpagesize() - sizeof(t_list);
		work_pointer = (char *)work_pointer + ceiling(((zone->max_size * 128) / getpagesize())) * getpagesize() - sizeof(t_list) - sizeof(size_t);
		((size_t *)work_pointer)[0] = ceiling(((zone->max_size * 128) / getpagesize())) * getpagesize() - sizeof(t_list);

	}
		return NULL;
}