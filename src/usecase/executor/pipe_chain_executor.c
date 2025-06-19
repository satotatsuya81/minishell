/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_chain_executor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 22:11:27 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "usecase/executor/executor.h"

static int	setup_pipe_resources_with_service(t_cmd *cmds, int **pipefd,
	pid_t **pids, t_process_service *proc_service)
{
	int	cmd_count;

	cmd_count = allocate_pipe_resources(cmds, pipefd, pids);
	if (cmd_count == -1 || create_pipes_with_service(*pipefd, cmd_count,
			proc_service) == -1)
	{
		free(*pipefd);
		free(*pids);
		return (-1);
	}
	return (cmd_count);
}

static int	setup_pipe_resources(t_cmd *cmds, int **pipefd, pid_t **pids)
{
	int	cmd_count;

	cmd_count = allocate_pipe_resources(cmds, pipefd, pids);
	if (cmd_count == -1 || create_pipes(*pipefd, cmd_count) == -1)
	{
		free(*pipefd);
		free(*pids);
		return (-1);
	}
	return (cmd_count);
}

static void	cleanup_and_free_resources(int *pipefd, pid_t *pids, int cmd_count)
{
	cleanup_pipes(pipefd, cmd_count);
	free(pipefd);
	free(pids);
}


static int	finalize_pipe_execution_with_service(int *pipefd, pid_t *pids,
	int cmd_count, t_process_service *proc_service)
{
	cleanup_pipes_with_service(pipefd, cmd_count, proc_service);
	cmd_count = wait_for_children_with_service(pids, cmd_count, proc_service);
	free(pipefd);
	free(pids);
	return (cmd_count);
}

static int	finalize_pipe_execution(int *pipefd, pid_t *pids, int cmd_count)
{
	cleanup_pipes(pipefd, cmd_count);
	cmd_count = wait_for_children(pids, cmd_count);
	free(pipefd);
	free(pids);
	return (cmd_count);
}

int	execute_pipe_chain_with_service(t_cmd *cmds, t_exec_context *ctx)
{
	int					*pipefd;
	pid_t				*pids;
	int					cmd_count;
	t_cmd_loop_params	loop_params;

	if (!cmds || !ctx)
		return (EXIT_FAILURE);
	cmd_count = allocate_pipe_resources(cmds, &pipefd, &pids);
	if (cmd_count == 1)
		return (execute_single_command(cmds, ctx));
	cmd_count = setup_pipe_resources_with_service(cmds, &pipefd, &pids,
			ctx->process_service);
	if (cmd_count == -1)
		return (EXIT_FAILURE);
	loop_params.cmds = cmds;
	loop_params.pipefd = pipefd;
	loop_params.pids = pids;
	loop_params.ctx = ctx;
	loop_params.cmd_count = cmd_count;
	if (execute_commands_loop(&loop_params) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (finalize_pipe_execution_with_service(pipefd, pids, cmd_count,
			ctx->process_service));
}

int	execute_pipe_chain(t_cmd *cmds, t_exec_context *ctx)
{
	int					*pipefd;
	pid_t				*pids;
	int					cmd_count;
	t_cmd_loop_params	loop_params;

	if (!cmds || !ctx)
		return (EXIT_FAILURE);
	cmd_count = allocate_pipe_resources(cmds, &pipefd, &pids);
	if (cmd_count == 1)
		return (execute_single_command(cmds, ctx));
	cmd_count = setup_pipe_resources(cmds, &pipefd, &pids);
	if (cmd_count == -1)
		return (EXIT_FAILURE);
	loop_params.cmds = cmds;
	loop_params.pipefd = pipefd;
	loop_params.pids = pids;
	loop_params.ctx = ctx;
	loop_params.cmd_count = cmd_count;
	if (execute_commands_loop(&loop_params) == -1)
	{
		cleanup_and_free_resources(pipefd, pids, cmd_count);
		return (EXIT_FAILURE);
	}
	return (finalize_pipe_execution(pipefd, pids, cmd_count));
}
