/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/18 16:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "evaluation_test_suite.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

void	setup_test_environment(t_test_context *ctx)
{
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	ctx->stderr_backup = dup(STDERR_FILENO);
	ctx->temp_file = strdup("/tmp/minishell_test_XXXXXX");
	ctx->child_pid = 0;
	
	/* Create temporary file for testing */
	int fd = mkstemp(ctx->temp_file);
	if (fd != -1)
		close(fd);
}

void	cleanup_test_environment(t_test_context *ctx)
{
	/* Restore file descriptors */
	dup2(ctx->stdin_backup, STDIN_FILENO);
	dup2(ctx->stdout_backup, STDOUT_FILENO);
	dup2(ctx->stderr_backup, STDERR_FILENO);
	
	close(ctx->stdin_backup);
	close(ctx->stdout_backup);
	close(ctx->stderr_backup);
	
	/* Clean up temporary file */
	if (ctx->temp_file)
	{
		unlink(ctx->temp_file);
		free(ctx->temp_file);
	}
	
	/* Kill child process if still running */
	if (ctx->child_pid > 0)
	{
		kill(ctx->child_pid, SIGTERM);
		waitpid(ctx->child_pid, NULL, 0);
	}
}

t_test_result	*execute_minishell_command(const char *command, t_test_context *ctx)
{
	t_test_result	*result;
	int				pipefd_out[2];
	int				pipefd_err[2];
	int				pipefd_in[2];
	int				status;
	char			buffer[4096];
	ssize_t			bytes_read;

	result = malloc(sizeof(t_test_result));
	if (!result)
		return (NULL);
	
	result->stdout_output = NULL;
	result->stderr_output = NULL;
	result->exit_code = 0;
	result->signal_received = 0;

	/* Create pipes for communication */
	if (pipe(pipefd_in) == -1 || pipe(pipefd_out) == -1 || pipe(pipefd_err) == -1)
	{
		free(result);
		return (NULL);
	}

	ctx->child_pid = fork();
	if (ctx->child_pid == 0)
	{
		/* Child process */
		close(pipefd_in[1]);
		close(pipefd_out[0]);
		close(pipefd_err[0]);
		
		dup2(pipefd_in[0], STDIN_FILENO);
		dup2(pipefd_out[1], STDOUT_FILENO);
		dup2(pipefd_err[1], STDERR_FILENO);
		
		close(pipefd_in[0]);
		close(pipefd_out[1]);
		close(pipefd_err[1]);
		
		execl("./minishell", "minishell", NULL);
		exit(EXIT_FAILURE);
	}
	else if (ctx->child_pid > 0)
	{
		/* Parent process */
		close(pipefd_in[0]);
		close(pipefd_out[1]);
		close(pipefd_err[1]);
		
		/* Send command to minishell */
		write(pipefd_in[1], command, strlen(command));
		write(pipefd_in[1], "\nexit\n", 6);
		close(pipefd_in[1]);
		
		/* Read stdout */
		bytes_read = read(pipefd_out[0], buffer, sizeof(buffer) - 1);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			result->stdout_output = strdup(buffer);
		}
		close(pipefd_out[0]);
		
		/* Read stderr */
		bytes_read = read(pipefd_err[0], buffer, sizeof(buffer) - 1);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			result->stderr_output = strdup(buffer);
		}
		close(pipefd_err[0]);
		
		/* Wait for child to finish */
		waitpid(ctx->child_pid, &status, 0);
		ctx->child_pid = 0;
		
		if (WIFEXITED(status))
			result->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			result->signal_received = WTERMSIG(status);
			result->exit_code = 128 + result->signal_received;
		}
	}
	else
	{
		/* Fork failed */
		close(pipefd_in[0]);
		close(pipefd_in[1]);
		close(pipefd_out[0]);
		close(pipefd_out[1]);
		close(pipefd_err[0]);
		close(pipefd_err[1]);
		free(result);
		return (NULL);
	}

	return (result);
}

void	free_test_result(t_test_result *result)
{
	if (result)
	{
		free(result->stdout_output);
		free(result->stderr_output);
		free(result);
	}
}