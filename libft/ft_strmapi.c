/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 19:03:53 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 13:19:43 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*newstring;

	i = 0;
	if (!s || !f)
		return (NULL);
	newstring = malloc(ft_strlen(s) + 1);
	if (newstring != 0)
	{
		while (s[i])
		{
			newstring[i] = (*f)(i, s[i]);
			i++;
		}
		newstring[i] = 0;
	}
	return (newstring);
}
