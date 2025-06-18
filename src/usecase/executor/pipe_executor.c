/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:35:40 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "usecase/executor/executor.h"

static void	setup_input_fd(int input_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input failed");
			exit(EXIT_FAILURE);
		}
		close(input_fd);
	}
}

static void	setup_output_fd(int output_fd)
{
	if (output_fd != STDOUT_FILENO)
	{
		if (dup2(output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 output failed");
			exit(EXIT_FAILURE);
		}
		close(output_fd);
	}
}

static void	close_all_pipes(t_pipe_params *params)
{
	int	i;

	if (!params->pipefd)
		return ;
	i = 0;
	while (i < (params->cmd_count - 1) * 2)
	{
		if (params->pipefd[i] != params->input_fd 
			&& params->pipefd[i] != params->output_fd)
			close(params->pipefd[i]);
		i++;
	}
}

static void	execute_command_in_child(t_pipe_params *params)
{
	int	status;

	expand_command_variables(params->cmd, params->ctx);
	setup_input_fd(params->input_fd);
	setup_output_fd(params->output_fd);
	close_all_pipes(params);
	if (params->cmd->redirects)
		if (setup_redirections(params->cmd->redirects) != 0)
			exit(EXIT_FAILURE);
	if (is_builtin(params->cmd->argv[0]))
	{
		status = execute_builtin(params->cmd, params->ctx);
		exit(status);
	}
	else
	{
		status = execute_external(params->cmd, params->ctx);
		exit(status);
	}
}

int	execute_pipe_command(t_pipe_params *params)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_command_in_child(params);
	else if (pid < 0)
	{
		perror("fork failed");
		return (-1);
	}
	return (pid);
}
