/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/14 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "usecase/executor/executor.h"

int	handle_output_redirect_with_service(const char *filename, 
	t_process_service *proc_service)
{
	int	fd;

	if (!filename)
	{
		printf("minishell: syntax error: missing filename\n");
		return (-1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (proc_service->duplicate_fd(fd, STDOUT_FILENO) != PROCESS_SUCCESS)
	{
		perror("dup2 failed for output redirection");
		proc_service->close_fd(fd);
		return (-1);
	}
	proc_service->close_fd(fd);
	return (0);
}

int	handle_output_redirect(const char *filename)
{
	int	fd;

	if (!filename)
	{
		printf("minishell: syntax error: missing filename\n");
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

int	handle_append_redirect_with_service(const char *filename, 
	t_process_service *proc_service)
{
	int	fd;

	if (!filename)
	{
		printf("minishell: syntax error: missing filename\n");
		return (-1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (proc_service->duplicate_fd(fd, STDOUT_FILENO) != PROCESS_SUCCESS)
	{
		perror("dup2 failed for append redirection");
		proc_service->close_fd(fd);
		return (-1);
	}
	proc_service->close_fd(fd);
	return (0);
}

int	handle_append_redirect(const char *filename)
{
	int	fd;

	if (!filename)
	{
		printf("minishell: syntax error: missing filename\n");
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
