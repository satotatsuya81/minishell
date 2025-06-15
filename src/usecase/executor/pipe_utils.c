/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:35:01 by tatsato          ###   ########.fr       */
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
