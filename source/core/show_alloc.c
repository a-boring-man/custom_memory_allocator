#include "malloc.h"

void	debug_hexa(void *memory_location, size_t size) {
	size_t i = 0;
	t_memory_pointer ptr;
	ptr.as_void = memory_location;
	ft_printf("\n");
	ft_printf("pointer at : -%p-\n", memory_location);

	while (i < size) {
		ft_printf("%x ", (*(ptr.as_sizeT)));
		ptr.as_sizeT += 1;
		i += 1;
	}
	ft_printf("\n\n");
}

static void display_page(void *page) {
	t_memory_pointer working_pointer;
	working_pointer.as_void = page;

	working_pointer.as_char += PAGE_START_OVERHEAD - sizeof(size_t);
	while (*(working_pointer.as_sizeT) != 1) {
		if (*(working_pointer.as_sizeT) & 1) {
			ft_printf("allocated from : -%p- to -%p- : %d bytes\n", working_pointer.as_void, working_pointer.as_char + (*(working_pointer.as_sizeT) & -2), (*working_pointer.as_sizeT) & -2);
		}
		else {
			ft_printf("free from : -%p- to -%p- : %d bytes\n", working_pointer.as_void, working_pointer.as_char + (*(working_pointer.as_sizeT) & -2), (*(working_pointer.as_sizeT)) & -2);
		}
		working_pointer.as_char += *(working_pointer.as_sizeT) & -2;
	}
}

void	show_alloc_mem() {
	size_t	grimoire_page = 0;
	size_t	page;
	t_list	*current_page;

	ft_printf("\n");
	while (grimoire_page != 11) {
		page = 0;
		ft_printf("%d bytes or less : \n", grimoire[grimoire_page].max_size);
		current_page = grimoire[grimoire_page].page;
		while (current_page != NULL && current_page->next != grimoire[grimoire_page].page) {
			ft_printf("page : %d\n", page++);
			display_page(grimoire[grimoire_page].page);
			
		}
		if (grimoire[grimoire_page].page != NULL) {
			ft_printf("page : %d\n", page++);
			display_page(grimoire[grimoire_page].page);
		}
		grimoire_page += 1;
	}
	ft_printf("\n");
}