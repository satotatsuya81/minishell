/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_remover.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:56:01 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:45:31 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/env_variable.h"
#include "domain/env_initializer.h"
#include "utils/libft_custom.h"

void	env_remove(t_env_var **env, const char *key)
{
	t_env_var	*curr;
	t_env_var	*prev;

	if (!env || !*env || !key)
		return ;
	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
