/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 19:23:25 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/09 15:12:35 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *to, const void *from, size_t size)
{
	size_t	i;

	if (to <= from)
		ft_memcpy(to, from, size);
	else
	{
		i = size;
		while (i > 0)
		{
			*(char *)(to + i - 1) = *(char *)(from + i - 1);
			i--;
		}
	}
	return (to);
}
