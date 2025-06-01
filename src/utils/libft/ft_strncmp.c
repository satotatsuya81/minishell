/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:59:50 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/23 19:18:08 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	if (s1 == 0 || s2 == 0)
		return (0);
	while (*p1 && *p2 && n > 0)
	{
		if (*p1 != *p2)
			break ;
		p1++;
		p2++;
		n--;
	}
	if (n == 0)
	{
		return (0);
	}
	return (*p1 - *p2);
}
