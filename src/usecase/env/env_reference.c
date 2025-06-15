/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_reference.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:56:01 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:38:38 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/env_variable.h"
#include "usecase/env/env_manager.h"
#include "utils/libft_custom.h"

char	*env_get(const t_env_var *env, const char *key)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
