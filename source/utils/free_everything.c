
#include "malloc.h"
#include <sys/mman.h>

void	free_everything() {
	for (int i = 0; i < 11; i++) {
		t_list *current_page = grimoire[i].page;
		while (current_page != NULL) {
			t_list *next = current_page->next;
			_remove_block_from_t_list(current_page, &(grimoire[i].page));
			munmap((void *)((size_t *)(current_page) - 1), *((size_t *)(current_page) - 1));
			if (current_page != next)
				current_page = next;
			else
				current_page = grimoire[i].page; // so NULL
		}
		grimoire[i].free = NULL;
	}
}