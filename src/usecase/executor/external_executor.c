/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:34:44 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "usecase/executor/executor.h"
#include "usecase/signal/signal_handler.h"
#include "interfaces/process_interface.h"

static int	execute_child_process(char *cmd_path, t_cmd *cmd, char **envp,
			t_process_service *proc_service)
{
	t_process_result	result;

	setup_child_signal_handlers();
	result = proc_service->exec_command(cmd_path, cmd->argv, envp);
	if (result != PROCESS_SUCCESS)
	{
		printf("minishell: execve failed\n");
		exit(EXIT_FAILURE);
	}
	return (EXIT_FAILURE);
}

static int	handle_parent_process(pid_t pid, char *cmd_path, char **envp,
			t_process_service *proc_service)
{
	int	status;
	t_process_result	result;

	ignore_signals();
	result = proc_service->wait_process(pid, &status);
	setup_signal_handlers();
	free(cmd_path);
	free_envp(envp);
	if (result != PROCESS_SUCCESS)
		return (EXIT_FAILURE);
	return (status);
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

static int	handle_fork_error(char *cmd_path, char **envp,
			t_process_service *proc_service)
{
	printf("minishell: %s\n",
		proc_service->get_error_message(PROCESS_ERROR_FORK));
	free(cmd_path);
	free_envp(envp);
	return (EXIT_FAILURE);
}

/* Execute external command */
int	execute_external(t_cmd *cmd, t_exec_context *ctx)
{
	pid_t				pid;
	char				*cmd_path;
	char				**envp;
	int					prep_result;
	t_process_result	result;
	t_process_service	*proc_service;

	proc_service = ctx->process_service;
	if (!proc_service)
		return (EXIT_FAILURE);
	prep_result = prepare_execution(cmd, ctx, &cmd_path, &envp);
	if (prep_result != EXIT_SUCCESS)
		return (prep_result);
	result = proc_service->fork_process(&pid);
	if (result == PROCESS_SUCCESS && pid == 0)
		return (execute_child_process(cmd_path, cmd, envp, proc_service));
	else if (result == PROCESS_SUCCESS && pid > 0)
		return (handle_parent_process(pid, cmd_path, envp, proc_service));
	return (handle_fork_error(cmd_path, envp, proc_service));
}
