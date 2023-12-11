/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 11:39:53 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 09:17:56 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *string, int c)
{
	while (*string)
	{
		if (*string == (char)c)
			return ((char *)string);
		string++;
	}
	if (*string == '\0' && c == '\0')
		return ((char *)string);
	return (NULL);
}
