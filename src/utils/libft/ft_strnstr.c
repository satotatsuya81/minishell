/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 10:41:01 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/25 00:12:19 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*s;
	char	*t;
	size_t	size;

	if (needle == 0)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	if (haystack == 0 || len < (size_t)ft_strlen(needle))
		return (NULL);
	size = len - (size_t)ft_strlen(needle) + 1;
	while (*haystack != '\0' && size > 0)
	{
		s = (char *)haystack;
		t = (char *)needle;
		while (*s && *t && *s++ == *t)
			t++;
		if (*t == '\0')
			return ((char *)haystack);
		haystack++;
		size--;
	}
	return (NULL);
}
