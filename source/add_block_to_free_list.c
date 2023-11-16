#include "malloc.h"

void	add_block_to_free_list(t_list *new_block, t_list **list_head) {
	t_list	*current = *list_head;
	if (*list_head == NULL) { // list is empty
		*list_head = new_block;
	}
	else if (current->next == current && current < new_block) { // list have one element and must insert at the end
		current->next = new_block;
		current->previous = new_block;
		new_block->next = current;
		new_block->previous = current;
	}
	else if (current->next == current) { // list have one element but must insert at the beginning
		current->next = new_block;
		current->previous = new_block;
		new_block->next = current;
		new_block->previous = current;
		*list_head = new_block;
	}
	else { // list have at least 2 element
		while (current->next != *list_head && (t_list *)current->next < new_block )
		{
			current = current->next;
		}
		if 	(current->next == *list_head) { // gone trough all the list must insert at the end
			(*list_head)->previous = new_block;
			new_block->next = *list_head;
			current->next = new_block;
			new_block->previous = current;
		}
	}
}