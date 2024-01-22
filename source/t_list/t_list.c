#include "malloc.h"

void    _printf_t_list(t_list *list_head) { // this is a debug function left here on purpose just in case something goes wrong during the defense and may induce some segfault due to debughexa
    t_list *current = list_head;
    size_t element_nbr = 0;

    if (current == NULL) {
        ft_printf("t_list contain %d elements\n", element_nbr);
        return;
    }
    while (current->next != list_head) {
        element_nbr++;
        ft_printf("t_list element nbr : %d is at adress %p\n", element_nbr, current);
        _debug_hexa(1, (void *)((size_t *)current -1), 25);
        current = current->next;
    }
    element_nbr++;
    ft_printf("t_list element nbr : %d is at adress %p\n", element_nbr, current);
    _debug_hexa(1, (void *)((size_t *)current -1), 25);
    ft_printf("the list contain %d elements\n", element_nbr);
}

void	_remove_block_from_t_list(t_list *block, t_list **head) {
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
	# ifdef POISON_FREE
		_poison_block((void *)block, sizeof(t_list), FREE_COLOR);
	# endif
}

void	_add_block_to_t_list(t_list *new_block, t_list **list_head) {
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