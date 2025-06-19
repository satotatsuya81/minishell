/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command_executor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:34:50 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "usecase/executor/executor.h"

static void	setup_pipe_params(t_pipe_params *pipe_params,
						t_cmd_loop_params *loop_params, int i)
{
	pipe_params->cmd = loop_params->cmds;
	pipe_params->ctx = loop_params->ctx;
	pipe_params->pipefd = loop_params->pipefd;
	pipe_params->cmd_count = loop_params->cmd_count;
	if (i > 0)
		pipe_params->input_fd = loop_params->pipefd[(i - 1) * 2];
	else
		pipe_params->input_fd = STDIN_FILENO;
	if (i < loop_params->cmd_count - 1)
		pipe_params->output_fd = loop_params->pipefd[i * 2 + 1];
	else
		pipe_params->output_fd = STDOUT_FILENO;
}

int	execute_commands_loop(t_cmd_loop_params *loop_params)
{
	t_pipe_params	pipe_params;
	int				i;

	i = 0;
	while (i < loop_params->cmd_count)
	{
		setup_pipe_params(&pipe_params, loop_params, i);
		loop_params->pids[i] = execute_pipe_command(&pipe_params);
		if (loop_params->pids[i] == -1)
			return (-1);
		loop_params->cmds = loop_params->cmds->next;
		i++;
	}
	return (0);
}
