#include "malloc.h"

void	add_block_to_t_list(t_list *new_block, t_list **list_head) {
	new_block->next = new_block; // circularise the block just in case i forgot to do it somwhere
	new_block->previous = new_block;

	if (*list_head == NULL)
		*list_head = new_block;
	else {
		t_list	*tmp = (*list_head)->previous;
		new_block->next = *list_head;
		new_block->previous = tmp;
		tmp->next = new_block;
		(*list_head)->previous = new_block;
		*list_head = new_block;
	}
}