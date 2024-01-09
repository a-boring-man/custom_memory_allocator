#include "malloc.h"

void	remove_block_from_t_list(t_list *block, t_list **head) {
	if (block == NULL)
		return;
	if (*head == block && block->next == block) { // one element
		*head = NULL;
	}
	else if (*head == block) { // the first element of a none 1 element lst
		block->previous->next = block->next;
		block->next->previous = block->previous;
		*head = block->next;
	}
	else { // not the first blockk and not a 1 element list
		block->previous->next = block->next;
		block->next->previous = block->previous;
	}
	block->next = NULL;
	block->previous = NULL;
}