/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 18:21:26 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 20:37:33 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_min(size_t len, char const *s)
{
	if (ft_strlen(s) > len)
		return (len);
	else
		return (ft_strlen(s));
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	j;

	if (!s)
		return (NULL);
	substr = ft_calloc(ft_min(len, s) + 1, sizeof(*s));
	i = 0;
	j = 0;
	if (substr != 0)
	{
		while (s[i])
		{
			if (i >= start && (i - start) < len)
			{
				substr[j] = s[i];
				j++;
			}
			i++;
		}
		substr[j] = 0;
	}
	return (substr);
}
