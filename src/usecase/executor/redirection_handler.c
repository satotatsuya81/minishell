/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:35:11 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "usecase/executor/executor.h"
#include "domain/token.h"

static int	process_redirection_with_service(t_cmd_redirect *current, 
	t_process_service *proc_service)
{
	if (current->type == REDIRECT_INPUT)
		return (handle_input_redirect_with_service(current->file, proc_service));
	if (current->type == REDIRECT_OUTPUT)
		return (handle_output_redirect_with_service(current->file, proc_service));
	if (current->type == REDIRECT_APPEND)
		return (handle_append_redirect_with_service(current->file, proc_service));
	if (current->type == REDIRECT_HEREDOC)
		return (handle_heredoc_redirect_with_service(current->file, proc_service));
	printf("minishell: unsupported redirection type\n");
	return (-1);
}

static int	process_redirection(t_cmd_redirect *current)
{
	if (current->type == REDIRECT_INPUT)
		return (handle_input_redirect(current->file));
	if (current->type == REDIRECT_OUTPUT)
		return (handle_output_redirect(current->file));
	if (current->type == REDIRECT_APPEND)
		return (handle_append_redirect(current->file));
	if (current->type == REDIRECT_HEREDOC)
		return (handle_heredoc_redirect(current->file));
	printf("minishell: unsupported redirection type\n");
	return (-1);
}

int	setup_redirections_with_service(t_cmd_redirect *redirects, 
	t_process_service *proc_service)
{
	t_cmd_redirect	*current;

	current = redirects;
	while (current)
	{
		if (process_redirection_with_service(current, proc_service) != 0)
			return (-1);
		current = current->next;
	}
	return (0);
}

int	setup_redirections(t_cmd_redirect *redirects)
{
	t_cmd_redirect	*current;

	current = redirects;
	while (current)
	{
		if (process_redirection(current) != 0)
			return (-1);
		current = current->next;
	}
	return (0);
}

void	restore_redirections_with_service(int saved_stdin, int saved_stdout, 
	t_process_service *proc_service)
{
	if (saved_stdin != -1)
	{
		proc_service->duplicate_fd(saved_stdin, STDIN_FILENO);
		proc_service->close_fd(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		proc_service->duplicate_fd(saved_stdout, STDOUT_FILENO);
		proc_service->close_fd(saved_stdout);
	}
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
