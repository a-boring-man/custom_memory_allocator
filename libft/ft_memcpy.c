/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 18:48:55 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 09:07:40 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *to, const void *from, size_t size)
{
	size_t	i;

	i = 0;
	if (!to && ! from)
		return (0);
	while (i < size)
	{
		*(char *)(to + i) = *(char *)(from + i);
		i++;
	}
	return (to);
}
