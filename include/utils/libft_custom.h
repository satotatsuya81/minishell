/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_custom.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:02:42 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 08:41:33 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_CUSTOM_H
# define LIBFT_CUSTOM_H
# include "libft.h"

char		*ft_strndup(const char *s, size_t n);
size_t		ft_strnlen(const char *s, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
long long	my_atoll(const char *str);
#endif
