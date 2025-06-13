/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/13 23:58:47 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "usecase/executor/executor.h"
#include "domain/env_variable.h"
#include "utils/libft_custom.h"

/* Convert environment variables to envp array */
char	**env_to_envp(t_env_var *env)
{
	char		**envp;
	t_env_var	*current;
	int			count;
	int			i;
	char		*temp;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	current = env;
	while (current && i < count)
	{
		envp[i] = ft_strjoin(current->key, "=");
		if (envp[i] && current->value)
		{
			temp = envp[i];
			envp[i] = ft_strjoin(temp, current->value);
			free(temp);
		}
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

/* Find command path using PATH environment variable */
char	*find_command_path(const char *cmd, t_env_var *env)
{
	char		*path_env;
	char		**paths;
	char		*full_path;
	char		*temp;
	int			i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = env_get(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (temp)
		{
			full_path = ft_strjoin(temp, cmd);
			free(temp);
			if (full_path && access(full_path, X_OK) == 0)
			{
				free_envp(paths);
				return (full_path);
			}
			free(full_path);
		}
		i++;
	}
	free_envp(paths);
	return (NULL);
}

/* Execute external command */
int	execute_external(t_cmd *cmd, t_exec_context *ctx)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**envp;

	if (!cmd || !cmd->argv || !cmd->argv[0] || !ctx)
		return (EXIT_FAILURE);
	cmd_path = find_command_path(cmd->argv[0], *ctx->env);
	if (!cmd_path)
	{
		printf("minishell: %s: command not found\n", cmd->argv[0]);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	envp = env_to_envp(*ctx->env);
	if (!envp)
	{
		free(cmd_path);
		return (EXIT_FAILURE);
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, cmd->argv, envp) == -1)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		free_envp(envp);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (EXIT_FAILURE);
	}
	else
	{
		perror("fork failed");
		free(cmd_path);
		free_envp(envp);
		return (EXIT_FAILURE);
	}

	return (EXIT_FAILURE);
}
