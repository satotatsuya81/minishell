/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 08:31:42 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/22 22:23:59 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*tmp;
	const char	*s;

	if (dst == NULL && src == NULL)
		return (NULL);
	if (dst <= src)
	{
		tmp = (char *)dst;
		s = (char *)src;
		while (len--)
			*tmp++ = *s++;
	}
	else
	{
		tmp = (char *)dst;
		tmp += len;
		s = (char *)src;
		s += len;
		while (len--)
			*--tmp = *--s;
	}
	return (dst);
}
