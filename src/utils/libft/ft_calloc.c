/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:57:49 by tatsato           #+#    #+#             */
/*   Updated: 2024/04/22 20:33:45 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*mem;
	size_t	i;
	size_t	bytes;

	bytes = count * size;
	if (count != 0)
	{
		if (bytes / count != size)
			return (NULL);
	}
	mem = (char *)malloc(bytes);
	if (mem == NULL)
		return (NULL);
	i = 0;
	while (i < bytes)
		mem[i++] = 0;
	return ((void *)mem);
}
