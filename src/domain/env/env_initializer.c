/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_initializer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:56:01 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/14 09:43:06 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "domain/env_initializer.h"
#include "utils/libft_custom.h"

static char	*extract_key(const char *env_str)
{
	size_t	len;

	len = 0;
	while (env_str[len] && env_str[len] != '=')
		len++;
	return (ft_strndup(env_str, len));
}

static char	*extract_value(const char *env_str)
{
	char	*eq;

	eq = ft_strchr(env_str, '=');
	if (!eq)
		return (NULL);
	return (ft_strndup(eq + 1, ft_strlen(eq + 1)));
}

t_env_var	*env_create_from_envp(char **envp)
{
	t_env_var	*head;
	char		*key;
	char		*value;
	int			i;

	if (!envp)
		return (NULL);
	head = NULL;
	i = 0;
	while (envp[i])
	{
		key = extract_key(envp[i]);
		value = extract_value(envp[i]);
		if (value == NULL)
		{
			free(key);
			i++;
			continue ;
		}
		env_add(&head, key, value, "=");
		free(key);
		free(value);
		i++;
	}
	return (head);
}

void	env_free(t_env_var *env)
{
	t_env_var	*tmp;

	if (!env)
		return ;
	while (env)
	{
		tmp = env;
		env = env->next;
		if (!tmp)
			continue ;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
		tmp = NULL;
	}
}
