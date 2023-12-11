/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:55:49 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 13:18:31 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_string;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	new_string = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (new_string != 0)
	{
		i = 0;
		j = 0;
		while (s1[i])
			new_string[j++] = s1[i++];
		i = 0;
		while (s2[i])
			new_string[j++] = s2[i++];
		new_string[j] = 0;
	}
	return (new_string);
}
