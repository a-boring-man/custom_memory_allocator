/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 12:44:33 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 09:20:30 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *string, int c)
{
	int	i;

	i = ft_strlen(string);
	if (string[i] == '\0' && c == '\0')
		return ((char *)&string[i]);
	while (i > 0)
	{
		if (string[i - 1] == (char)c)
			return ((char *)&string[i - 1]);
		i--;
	}
	return (NULL);
}
