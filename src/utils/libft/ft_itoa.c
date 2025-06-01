/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 20:08:26 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/24 23:35:38 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_digits(long n);
static char	*get_nbr(char *c, int digits, long nbr);

char	*ft_itoa(int n)
{
	int		digits;
	int		sign;
	char	*c;
	long	nbr;

	sign = 1;
	nbr = (long)n;
	if (nbr < 0)
	{
		sign = -1;
		nbr *= -1;
	}
	digits = get_digits(nbr);
	if (sign < 0)
		digits++;
	c = (char *)malloc(sizeof(char) * (digits + 1));
	if (c == NULL)
		return (NULL);
	c = get_nbr(c, digits, nbr);
	if (sign < 0)
		c[0] = '-';
	c[digits] = '\0';
	return (c);
}

static int	get_digits(long n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*get_nbr(char *c, int digits, long nbr)
{
	while (--digits >= 0)
	{
		c[digits] = nbr % 10 + '0';
		nbr /= 10;
	}
	return (c);
}
