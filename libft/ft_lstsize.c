/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 15:38:00 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 16:56:17 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		counter;
	t_list	*ptr;

	counter = 1;
	ptr = lst;
	if (!lst)
		return (0);
	while (ptr->next)
	{
		ptr = ptr->next;
		counter++;
	}
	return (counter);
}
