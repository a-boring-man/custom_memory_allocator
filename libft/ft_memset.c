/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 17:57:19 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/09 10:12:45 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *block, int c, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		*(unsigned char *)(block + i) = (unsigned char)c;
		i++;
	}
	return (block);
}
