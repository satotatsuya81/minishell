/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 21:18:27 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/24 23:30:08 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	write_nbr(long nbr, int fd);

void	ft_putnbr_fd(int n, int fd)
{
	long	nbr;

	nbr = n;
	if (n < 0)
	{
		write(fd, "-", 1);
		nbr *= -1;
	}
	write_nbr(nbr, fd);
}

static void	write_nbr(long nbr, int fd)
{
	char	c;

	if (nbr >= 10)
		write_nbr(nbr / 10, fd);
	c = nbr % 10 + '0';
	write(fd, &c, 1);
}
