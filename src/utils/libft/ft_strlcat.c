/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:42:18 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/24 23:31:24 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_strnlen(char *s, size_t maxlen);

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	char	*d;
	size_t	destlen;
	size_t	srclen;
	size_t	tmp;

	destlen = ft_strnlen(dest, size);
	srclen = ft_strlen(src);
	if (destlen == size)
		return (destlen + srclen);
	d = dest;
	tmp = size;
	while (tmp-- != 0 && *d != '\0')
		d++;
	while (*src != '\0')
	{
		if (destlen < size - 1)
		{
			*d++ = *src;
			size--;
		}
		src++;
	}
	*d = '\0';
	return (destlen + srclen);
}

static size_t	ft_strnlen(char *s, size_t maxlen)
{
	size_t	len;

	len = 0;
	while (len < maxlen && *s++)
	{
		len++;
	}
	return (len);
}
