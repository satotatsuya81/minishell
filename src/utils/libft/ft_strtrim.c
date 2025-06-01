/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 22:43:43 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/24 23:32:22 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_include(char const c, char const *str);
static int	get_start(char const *s1, char const *set);
static int	get_count(char const *s1, char const *set);

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*mem;
	int		i;
	int		len;
	int		count;

	len = ft_strlen(s1) - get_start(s1, set);
	s1 = s1 + get_start(s1, set);
	count = get_count(s1, set);
	if (len - count < 0)
	{
		mem = (char *)malloc(sizeof(char));
		mem[0] = '\0';
		return (mem);
	}
	mem = (char *)malloc(sizeof(char) * (len - count) + 1);
	if (mem == NULL)
		return (NULL);
	i = 0;
	while (i < len - count && *s1)
		mem[i++] = *s1++;
	mem[i] = '\0';
	return (mem);
}

static int	is_include(char const c, char const *str)
{
	while (*str)
	{
		if (c == *str)
			return (1);
		str++;
	}
	return (0);
}

static int	get_start(char const *s1, char const *set)
{
	int	count;

	count = 0;
	while (*s1 && is_include(*s1, set))
	{
		s1++;
		count++;
	}
	return (count);
}

static int	get_count(char const *s1, char const *set)
{
	int	count;

	count = 0;
	while (*s1)
	{
		if (is_include(*s1++, set))
			count++;
		else
			count = 0;
	}
	return (count);
}
