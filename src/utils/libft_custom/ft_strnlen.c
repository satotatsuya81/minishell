/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:05:29 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 16:05:45 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft_custom.h"

size_t	ft_strnlen(const char *s, size_t n)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len] && len < n)
		len++;
	return (len);
}
