/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:41:24 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/10 14:13:46 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *a1, const void *a2, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (0);
	while (((unsigned char *)a1)[i] == ((unsigned char *)a2)[i] && i < size - 1)
	{
		i++;
	}
	return (((unsigned char *)a1)[i] - ((unsigned char *)a2)[i]);
}
