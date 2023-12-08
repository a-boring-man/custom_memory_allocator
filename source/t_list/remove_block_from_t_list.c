#include "malloc.h"

void	remove_block_from_t_list(t_list *block, t_list **head) {
	t_list	*current = *head;

	if (current == block && current->next == block) { // one element
		*head = NULL;
		return;
	}
	else if (current == block) { // the first element of a none 1 element lst
		current->previous->next = current->next;
		current->next->previous = current->previous;
		*head = current->next;
	}
	else { // not the first blockk and not a 1 element list
		while (current->next != block)
		{
			current = current->next;
		}
		current->next = block->next;
		block->next->previous = current;
	}
}