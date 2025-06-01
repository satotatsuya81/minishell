/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 10:08:46 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/22 19:32:56 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;
	int		flg;

	flg = 0;
	while (*s)
	{
		if (*s == (char)c)
		{
			flg = 1;
			ptr = (char *)s;
		}
		s++;
	}
	if (flg)
		return (ptr);
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}
