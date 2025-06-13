/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/13 23:19:59 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "usecase/executor/executor.h"

/* Count commands in pipe chain */
static int	count_commands(t_cmd *cmds)
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

/* Execute single command in pipe */
static int	execute_pipe_command(t_cmd *cmd, int input_fd, int output_fd, t_exec_context *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		/* Child process */

		/* Setup input */
		if (input_fd != STDIN_FILENO)
		{
			if (dup2(input_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 input failed");
				exit(EXIT_FAILURE);
			}
			close(input_fd);
		}

		/* Setup output */
		if (output_fd != STDOUT_FILENO)
		{
			if (dup2(output_fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 output failed");
				exit(EXIT_FAILURE);
			}
			close(output_fd);
		}

		/* Setup redirections if any */
		if (cmd->redirects && setup_redirections(cmd->redirects) != 0)
			exit(EXIT_FAILURE);

		/* Execute command */
		if (is_builtin(cmd->argv[0]))
		{
			int status = execute_builtin(cmd, ctx);
			exit(status);
		}
		else
		{
			int status = execute_external(cmd, ctx);
			exit(status);
		}
	}
	else if (pid < 0)
	{
		perror("fork failed");
		return (-1);
	}

	return (pid);
}

/* Execute chain of commands connected by pipes */
int	execute_pipe_chain(t_cmd *cmds, t_exec_context *ctx)
{
	int		cmd_count;
	int		*pipefd;
	pid_t	*pids;
	t_cmd	*current;
	int		i;
	int		status;
	int		last_status;

	if (!cmds || !ctx)
		return (EXIT_FAILURE);

	cmd_count = count_commands(cmds);
	if (cmd_count == 1)
		return (execute_single_command(cmds, ctx));

	/* Allocate arrays for pipes and process IDs */
	pipefd = malloc(sizeof(int) * 2 * (cmd_count - 1));
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipefd || !pids)
	{
		free(pipefd);
		free(pids);
		return (EXIT_FAILURE);
	}

	/* Create all pipes */
	for (i = 0; i < cmd_count - 1; i++)
	{
		if (pipe(pipefd + i * 2) == -1)
		{
			perror("pipe failed");
			free(pipefd);
			free(pids);
			return (EXIT_FAILURE);
		}
	}

	/* Execute commands */
	current = cmds;
	for (i = 0; i < cmd_count; i++)
	{
		int input_fd = STDIN_FILENO;
		int output_fd = STDOUT_FILENO;

		/* Setup input from previous pipe */
		if (i > 0)
			input_fd = pipefd[(i - 1) * 2];

		/* Setup output to next pipe */
		if (i < cmd_count - 1)
			output_fd = pipefd[i * 2 + 1];

		/* Execute command */
		pids[i] = execute_pipe_command(current, input_fd, output_fd, ctx);
		if (pids[i] == -1)
		{
			/* Cleanup on failure */
			for (int j = 0; j < cmd_count - 1; j++)
			{
				close(pipefd[j * 2]);
				close(pipefd[j * 2 + 1]);
			}
			free(pipefd);
			free(pids);
			return (EXIT_FAILURE);
		}

		current = current->next;
	}

	/* Close all pipe file descriptors in parent */
	for (i = 0; i < cmd_count - 1; i++)
	{
		close(pipefd[i * 2]);
		close(pipefd[i * 2 + 1]);
	}

	/* Wait for all children */
	last_status = EXIT_SUCCESS;
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)  /* Status of last command is the pipe status */
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else
				last_status = EXIT_FAILURE;
		}
	}

	free(pipefd);
	free(pids);
	return (last_status);
}

