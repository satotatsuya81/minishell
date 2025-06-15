/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_converter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/13 23:58:47 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "usecase/executor/executor.h"
#include "utils/libft_custom.h"

static int	count_env_vars(t_env_var *env)
{
	int			count;
	t_env_var	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env_var *env_var)
{
	char	*env_str;
	char	*temp;

	env_str = ft_strjoin(env_var->key, "=");
	if (env_str && env_var->value)
	{
		temp = env_str;
		env_str = ft_strjoin(temp, env_var->value);
		free(temp);
	}
	return (env_str);
}

/* Convert environment variables to envp array */
char	**env_to_envp(t_env_var *env)
{
	char		**envp;
	t_env_var	*current;
	int			count;
	int			i;

	count = count_env_vars(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	current = env;
	while (current && i < count)
	{
		envp[i] = create_env_string(current);
		if (!envp[i])
		{
			free_envp(envp);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

/* Free envp array */
void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
