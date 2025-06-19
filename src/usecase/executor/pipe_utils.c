/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 22:10:31 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "usecase/executor/executor.h"
#include "usecase/signal/signal_handler.h"
#include "interfaces/process_interface.h"

/* Count commands in pipe chain */
int	count_commands(t_cmd *cmds)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmds;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	allocate_pipe_resources(t_cmd *cmds, int **pipefd, pid_t **pids)
{
	int	cmd_count;

	cmd_count = count_commands(cmds);
	if (cmd_count == 1)
		return (1);
	*pipefd = malloc(sizeof(int) * 2 * (cmd_count - 1));
	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pipefd || !*pids)
	{
		free(*pipefd);
		free(*pids);
		return (-1);
	}
	return (cmd_count);
}

int	create_pipes_with_service(int *pipefd, int cmd_count,
	t_process_service *proc_service)
{
	int					i;
	t_pipe_info			pipe_info;
	t_process_result	result;

	i = 0;
	while (i < cmd_count - 1)
	{
		result = proc_service->create_pipe(&pipe_info);
		if (result != PROCESS_SUCCESS)
		{
			perror("pipe failed");
			return (-1);
		}
		pipefd[i * 2] = pipe_info.read_fd;
		pipefd[i * 2 + 1] = pipe_info.write_fd;
		i++;
	}
	return (0);
}

/* Deprecated - use create_pipes_with_service with exec context instead */
int	create_pipes(int *pipefd, int cmd_count)
{
	t_process_service	*proc_service;
	int					result;

	proc_service = create_process_service();
	if (!proc_service)
		return (-1);
	result = create_pipes_with_service(pipefd, cmd_count, proc_service);
	destroy_process_service(proc_service);
	return (result);
}

void	cleanup_pipes_with_service(int *pipefd, int cmd_count,
	t_process_service *proc_service)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		proc_service->close_fd(pipefd[i * 2]);
		proc_service->close_fd(pipefd[i * 2 + 1]);
		i++;
	}
}

/* Deprecated - use cleanup_pipes_with_service with exec context instead */
void	cleanup_pipes(int *pipefd, int cmd_count)
{
	t_process_service	*proc_service;

	proc_service = create_process_service();
	if (!proc_service)
		return ;
	cleanup_pipes_with_service(pipefd, cmd_count, proc_service);
	destroy_process_service(proc_service);
}

int	wait_for_children_with_service(pid_t *pids, int cmd_count,
	t_process_service *proc_service)
{
	int	i;
	int	status;
	int	last_status;

	last_status = EXIT_SUCCESS;
	i = 0;
	while (i < cmd_count)
	{
		if (proc_service->wait_process(pids[i], &status) != PROCESS_SUCCESS)
		{
			perror("wait failed for child process");
			return (EXIT_FAILURE);
		}
		if (i == cmd_count - 1)
			last_status = status;
		i++;
	}
	return (last_status);
}

/* Deprecated - use wait_for_children_with_service with exec context instead */
int	wait_for_children(pid_t *pids, int cmd_count)
{
	t_process_service	*proc_service;
	int					result;

	proc_service = create_process_service();
	if (!proc_service)
		return (EXIT_FAILURE);
	result = wait_for_children_with_service(pids, cmd_count, proc_service);
	destroy_process_service(proc_service);
	return (result);
}
