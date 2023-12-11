/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 20:03:01 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/15 10:23:35 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	true_dest_size;
	size_t	source_lenght;

	i = 0;
	source_lenght = ft_strlen(src);
	while (dest[i])
	{
		i++;
	}
	true_dest_size = i;
	if (i >= size)
		return (size + source_lenght);
	else
	{
		while ((i + 1) < size && *src != '\0')
		{
			dest[i] = *src;
			src++;
			i++;
		}
		dest[i] = '\0';
		return (true_dest_size + source_lenght);
	}
} */

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	srcl;
	size_t	destl;

	srcl = ft_strlen(src);
	destl = ft_strlen(dest);
	if (destl >= size)
		return (size + srcl);
	i = destl;
	while (i + 1 < size && *src)
	{
		dest[i] = *src;
		src++;
		i++;
	}
	dest[i] = '\0';
	return (destl + srcl);
}

/* int main()
{
	char dest[15] = "";
	ft_bzero(dest, 15);
	memset(dest, 'r', 6);
	char *src = "lorem";
	dest[11] = 'a';
	printf("%zu, ", ft_strlcat(dest, src, 15));
	printf("%s, ", dest);
} */
