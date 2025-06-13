/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claude <claude@anthropic.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/01/06 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "usecase/executor/executor.h"
#include "domain/token.h"

/* Handle input redirection (<) */
static int	handle_input_redirect(const char *filename)
{
	int	fd;

	if (!filename)
	{
		printf("minishell: syntax error: missing filename for input redirection\n");
		return (-1);
	}

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}

	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 failed for input redirection");
		close(fd);
		return (-1);
	}

	close(fd);
	return (0);
}

/* Handle output redirection (>) */
static int	handle_output_redirect(const char *filename)
{
	int	fd;

	if (!filename)
	{
		printf("minishell: syntax error: missing filename for output redirection\n");
		return (-1);
	}

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}

	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed for output redirection");
		close(fd);
		return (-1);
	}

	close(fd);
	return (0);
}

/* Handle append redirection (>>) */
static int	handle_append_redirect(const char *filename)
{
	int	fd;

	if (!filename)
	{
		printf("minishell: syntax error: missing filename for append redirection\n");
		return (-1);
	}

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}

	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed for append redirection");
		close(fd);
		return (-1);
	}

	close(fd);
	return (0);
}

/* Handle heredoc redirection (<<) */
static int	handle_heredoc_redirect(const char *delimiter)
{
	char	*line;
	size_t	delimiter_len;
	int		pipefd[2];
	pid_t	pid;

	if (!delimiter)
	{
		printf("minishell: syntax error: missing delimiter for heredoc\n");
		return (-1);
	}

	if (pipe(pipefd) == -1)
	{
		perror("pipe failed for heredoc");
		return (-1);
	}

	pid = fork();
	if (pid == 0)
	{
		/* Child process: read heredoc input */
		close(pipefd[0]);
		delimiter_len = strlen(delimiter);
		
		printf("heredoc> ");
		while ((line = readline("")) != NULL)
		{
			if (strlen(line) == delimiter_len && 
				strncmp(line, delimiter, delimiter_len) == 0)
			{
				free(line);
				break;
			}
			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
			printf("heredoc> ");
		}
		close(pipefd[1]);
		exit(0);
	}
	else if (pid > 0)
	{
		/* Parent process: setup input from pipe */
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed for heredoc");
			close(pipefd[0]);
			return (-1);
		}
		close(pipefd[0]);
		return (0);
	}
	else
	{
		perror("fork failed for heredoc");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
}

/* Setup all redirections for a command */
int	setup_redirections(t_cmd_redirect *redirects)
{
	t_cmd_redirect	*current;

	current = redirects;
	while (current)
	{
		switch (current->type)
		{
			case REDIRECT_INPUT:
				if (handle_input_redirect(current->file) != 0)
					return (-1);
				break;
			case REDIRECT_OUTPUT:
				if (handle_output_redirect(current->file) != 0)
					return (-1);
				break;
			case REDIRECT_APPEND:
				if (handle_append_redirect(current->file) != 0)
					return (-1);
				break;
			case REDIRECT_HEREDOC:
				if (handle_heredoc_redirect(current->file) != 0)
					return (-1);
				break;
			default:
				printf("minishell: unsupported redirection type\n");
				return (-1);
		}
		current = current->next;
	}

	return (0);
}

/* Restore original file descriptors */
void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}