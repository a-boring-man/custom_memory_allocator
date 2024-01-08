#include "malloc.h"

// static void	add_block_to_t_list_address_ordered(t_list *new_block, t_list **list_head) {
// 	t_list	*current = *list_head;
// 	if (*list_head == NULL) { // list is empty
// 		*list_head = new_block;
// 	}
// 	else if (current->next == current && current < new_block) { // list have one element and must insert at the end
// 		current->next = new_block;
// 		current->previous = new_block;
// 		new_block->next = current;
// 		new_block->previous = current;
// 	}
// 	else if (current->next == current) { // list have one element but must insert at the beginning
// 		current->next = new_block;
// 		current->previous = new_block;
// 		new_block->next = current;
// 		new_block->previous = current;
// 		*list_head = new_block;
// 	}
// 	else { // list have at least 2 element
// 		while (current->next != *list_head && current->next < new_block )
// 		{
// 			current = current->next;
// 		}
// 		if 	(current->next == *list_head) { // gone trough all the list must insert at the end
// 			(*list_head)->previous = new_block;
// 			new_block->next = *list_head;
// 			current->next = new_block;
// 			new_block->previous = current;
// 		}
// 	}
// }

static void	add_block_to_t_list_first(t_list *new_block, t_list **list_head) {
	new_block->next = new_block;
	new_block->previous = new_block;
	if (*list_head == NULL) {
		*list_head = new_block;
		ft_printf("list was null\n");
	}
	else {
		t_list	*tmp = (*list_head)->previous;
		new_block->next = *list_head;
		new_block->previous = tmp;
		tmp->next = new_block;
		(*list_head)->previous = new_block;
		*list_head = new_block;
	}
}

void	add_block_to_t_list(t_list *new_block, t_list **list_head) {
	add_block_to_t_list_first(new_block, list_head);
	//add_block_to_t_list_address_ordered(new_block, list_head);
}