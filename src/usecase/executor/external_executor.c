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
#include "usecase/executor/executor.h"

static int	execute_child_process(char *cmd_path, t_cmd *cmd, char **envp)
{
	if (execve(cmd_path, cmd->argv, envp) == -1)
	{
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	return (EXIT_FAILURE);
}

static int	handle_parent_process(pid_t pid, char *cmd_path, char **envp)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	free_envp(envp);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

static int	prepare_execution(t_cmd *cmd, t_exec_context *ctx,
							char **cmd_path, char ***envp)
{
	if (!cmd || !cmd->argv || !cmd->argv[0] || !ctx)
		return (EXIT_FAILURE);
	*cmd_path = find_command_path(cmd->argv[0], *ctx->env);
	if (!*cmd_path)
	{
		printf("minishell: %s: command not found\n", cmd->argv[0]);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	*envp = env_to_envp(*ctx->env);
	if (!*envp)
	{
		free(*cmd_path);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	handle_fork_error(char *cmd_path, char **envp)
{
	perror("fork failed");
	free(cmd_path);
	free_envp(envp);
	return (EXIT_FAILURE);
}

/* Execute external command */
int	execute_external(t_cmd *cmd, t_exec_context *ctx)
{
	pid_t	pid;
	char	*cmd_path;
	char	**envp;
	int		prep_result;

	prep_result = prepare_execution(cmd, ctx, &cmd_path, &envp);
	if (prep_result != EXIT_SUCCESS)
		return (prep_result);
	pid = fork();
	if (pid == 0)
		return (execute_child_process(cmd_path, cmd, envp));
	else if (pid > 0)
		return (handle_parent_process(pid, cmd_path, envp));
	return (handle_fork_error(cmd_path, envp));
}
