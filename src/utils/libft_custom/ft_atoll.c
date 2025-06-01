/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 08:28:11 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 10:44:04 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft_custom.h"
#include <limits.h>

long long	convert_str_to_positive_num(const char *str)
{
	long long	result;
	int			digit;

	result = 0;
	while (ft_isdigit(*str))
	{
		digit = *str - '0';
		if (result > (LLONG_MAX - digit) / 10)
			return (LLONG_MAX);
		result = result * 10 + digit;
		str++;
	}
	return (result);
}

long long	convert_str_to_negative_num(const char *str)
{
	long long	result;
	int			digit;

	result = 0;
	while (ft_isdigit(*str))
	{
		digit = *str - '0';
		if (result < (LLONG_MIN + digit) / 10)
			return (LLONG_MIN);
		result = result * 10 - digit;
		str++;
	}
	return (result);
}

long long	my_atoll(const char *str)
{
	int			negative;

	negative = 0;
	if (str == NULL)
		return (0);
	if (*str == '-')
	{
		negative = 1;
		str++;
	}
	else if (*str == '+')
		str++;
	if (negative)
		return (convert_str_to_negative_num(str));
	return (convert_str_to_positive_num(str));
}
