/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 11:38:41 by jrinna            #+#    #+#             */
/*   Updated: 2021/11/12 13:31:21 by jrinna           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nbr;
	int		r;

	nbr = n;
	if (fd > -1)
	{
		if (nbr < 0)
		{
			ft_putchar_fd('-', fd);
			nbr = -nbr;
		}
		r = nbr % 10;
		if (nbr > 9)
			ft_putnbr_fd(nbr / 10, fd);
		ft_putchar_fd((r + '0'), fd);
	}
}
