/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:02:04 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/24 23:32:39 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_size(size_t str_len, size_t start, size_t len);

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*mem;
	size_t	i;
	size_t	size;

	size = get_size(ft_strlen(s), (size_t)start, len);
	mem = (char *)malloc(size);
	if (mem == NULL)
		return (NULL);
	while (*s && start > 0)
	{
		s++;
		start--;
	}
	i = 0;
	while (*s && i < len)
		mem[i++] = *s++;
	mem[i] = '\0';
	return (mem);
}

static size_t	get_size(size_t str_len, size_t start, size_t len)
{
	size_t	size;

	if (str_len > start)
	{
		if ((str_len - start) < len)
			size = (str_len - start) * sizeof(char) + 1;
		else
			size = (len * sizeof(char) + 1);
	}
	else
		size = 1;
	return (size);
}
