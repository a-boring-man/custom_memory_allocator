/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 10:04:50 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 12:55:47 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isincharset(char c, const char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	i;
	int	end;

	start = 0;
	end = 0;
	i = 0;
	if (!s1)
		return (NULL);
	if (ft_strlen(s1) != 0)
		end = ft_strlen(s1) - 1;
	while (ft_isincharset(s1[start], set))
		start++;
	while (end && ft_isincharset(s1[end], set))
		end--;
	if (ft_strlen(s1) != 0 && start <= end)
	{
		return (ft_substr(s1, start, end - start + 1));
	}
	return (ft_calloc(1, 1));
}
