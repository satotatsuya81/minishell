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

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "usecase/executor/executor.h"

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

int	create_pipes(int *pipefd, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		if (pipe(pipefd + i * 2) == -1)
		{
			perror("pipe failed");
			return (-1);
		}
		i++;
	}
	return (0);
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

void	cleanup_pipes(int *pipefd, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipefd[i * 2]);
		close(pipefd[i * 2 + 1]);
		i++;
	}
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

int	wait_for_children(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	last_status;

	last_status = EXIT_SUCCESS;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else
				last_status = EXIT_FAILURE;
		}
		i++;
	}
	return (last_status);
}
