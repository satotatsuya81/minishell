/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 10:57:55 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/24 23:36:07 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strcmp(const char *s1, const char *s2);
static int	check_sign(const char **str);
static int	is_long_range(long a, int b);
static int	is_space(const char c);

int	ft_atoi(const char *str)
{
	long	num;
	long	long_max;
	int		sign;

	if (!ft_strcmp(str, "-9223372036854775808"))
		return ((int)0x8000000000000000L);
	num = 0;
	long_max = 9223372036854775807;
	while (is_space(*str))
		str++;
	sign = check_sign(&str);
	while (ft_isdigit(*str))
	{
		if (is_long_range(num, *str - '0'))
		{
			if (sign == -1)
				num = 0x8000000000000000L;
			else
				num = long_max;
			break ;
		}
		num = 10 * num + (*str++ - '0');
	}
	return ((int)(num * sign));
}

static int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (*p1 && *p2)
	{
		if (*p1 != *p2)
			break ;
		p1++;
		p2++;
	}
	return (*p1 - *p2);
}

static int	check_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		*str += 1;
	}
	return (sign);
}

static int	is_long_range(long a, int b)
{
	long	long_max;
	long	max_div;
	long	max_mod;

	long_max = 9223372036854775807;
	max_div = long_max / 10;
	max_mod = long_max % 10;
	return (a > max_div || (a == max_div && b > max_mod));
}

static int	is_space(const char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}
