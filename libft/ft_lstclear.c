/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 17:55:20 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 20:16:28 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*ptr1;

	if (!lst)
		return ;
	while (*lst)
	{
		ptr1 = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = ptr1;
	}
}
