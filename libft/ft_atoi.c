/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 10:52:26 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 08:59:16 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *string)
{
	int	i;
	int	counter;
	int	signe;
	int	nbr;

	i = 0;
	counter = 0;
	signe = 1;
	nbr = 0;
	while ((string[i] > 8 && string[i] < 14) || string[i] == 32)
		i++;
	while ((string[i] == 43 || string[i] == 45) && counter == 0)
	{
		counter++;
		if (string[i] == 45)
			signe = -signe;
		i++;
	}
	while (string[i] > 47 && string[i] < 58)
	{
		nbr = nbr * 10 + (string[i] - '0');
		i++;
	}
	return (signe * nbr);
}
