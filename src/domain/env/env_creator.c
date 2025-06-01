/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_creator.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:56:01 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:49:16 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/env_variable.h"
#include "domain/env_initializer.h"
#include "utils/libft_custom.h"

static t_env_var	*create_node(const char *key, const char *value)
{
	t_env_var	*new_node;

	new_node = malloc(sizeof(t_env_var));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

static char	*get_new_value(const char *current_value,
				const char *value, const char *operator)
{
	char	*new_value;

	if (ft_strcmp(operator, "+=") == 0)
	{
		if (current_value)
			new_value = ft_strjoin(current_value, value);
		else
			new_value = ft_strdup(value);
	}
	else if (ft_strcmp(operator, "=") == 0)
		new_value = ft_strdup(value);
	else
		new_value = NULL;
	return (new_value);
}

void	env_add(t_env_var **env,
			const char *key, const char *value, const char *operator)
{
	t_env_var	*curr;
	t_env_var	*new_node;
	char		*new_value;

	if (!env || !key)
		return ;
	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			new_value = get_new_value(curr->value, value, operator);
			free(curr->value);
			curr->value = new_value;
			return ;
		}
		curr = curr->next;
	}
	new_node = create_node(key, value);
	if (!new_node)
		return ;
	new_node->next = *env;
	*env = new_node;
}

void	env_add_assignment(t_env_var **env, t_assignment *assignment)
{
	if (!env || !assignment || !assignment->name)
		return ;
	env_add(env, assignment->name, assignment->value, assignment->operator);
}

t_env_var	*dup_env(const t_env_var *env)
{
	t_env_var	*new_env;
	t_env_var	*curr;
	t_env_var	*new_node;

	if (!env)
		return (NULL);
	new_env = NULL;
	curr = (t_env_var *)env;
	while (curr)
	{
		new_node = create_node(curr->key, curr->value);
		if (!new_node)
		{
			env_free(new_env);
			return (NULL);
		}
		new_node->next = new_env;
		new_env = new_node;
		curr = curr->next;
	}
	return (new_env);
}
