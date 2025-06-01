/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 01:51:23 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/26 13:52:44 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *str, char c)
{
	int	words;

	words = 0;
	if (!*str)
		return (0);
	while (*str)
	{
		if (*str == c)
		{
			str++;
			continue ;
		}
		words++;
		while (*str && *str != c)
			str++;
	}
	return (words);
}

static char	*worddup(char const *str, char c)
{
	char	*word;
	int		i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	word = malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	arr_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		i;

	i = 0;
	split = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!split || !s)
		return (NULL);
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (!*s)
			break ;
		split[i] = worddup(s, c);
		if (!split[i])
		{
			arr_free(split);
			return (NULL);
		}
		i++;
		while (*s && *s != c)
			s++;
	}
	split[i] = 0;
	return (split);
}
