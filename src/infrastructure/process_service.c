/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_service.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/16 22:09:21 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include "interfaces/process_interface.h"
#include "utils/libft_custom.h"

/* No global variables - using dependency injection */

static t_process_result	impl_fork_process(pid_t *pid)
{
	if (!pid)
		return (PROCESS_ERROR_FORK);
	*pid = fork();
	if (*pid == -1)
		return (PROCESS_ERROR_FORK);
	return (PROCESS_SUCCESS);
}

static t_process_result	impl_exec_command(const char *path, char **argv,
							char **envp)
{
	if (!path || !argv)
		return (PROCESS_ERROR_EXEC);
	if (execve(path, argv, envp) == -1)
		return (PROCESS_ERROR_EXEC);
	return (PROCESS_SUCCESS);
}

static t_process_result	impl_wait_process(pid_t pid, int *exit_status)
{
	int	status;

	if (pid <= 0)
		return (PROCESS_ERROR_WAIT);
	if (waitpid(pid, &status, 0) == -1)
		return (PROCESS_ERROR_WAIT);
	if (exit_status)
	{
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status = 128 + WTERMSIG(status);
		else
			*exit_status = 1;
	}
	return (PROCESS_SUCCESS);
}

static t_process_result	impl_wait_all_processes(pid_t *pids, int count,
							int *exit_statuses)
{
	int	i;
	int	status;

	if (!pids || count <= 0)
		return (PROCESS_ERROR_WAIT);
	i = 0;
	while (i < count)
	{
		if (impl_wait_process(pids[i], &status) != PROCESS_SUCCESS)
			return (PROCESS_ERROR_WAIT);
		if (exit_statuses)
			exit_statuses[i] = status;
		i++;
	}
	return (PROCESS_SUCCESS);
}

static t_process_result	impl_create_pipe(t_pipe_info *pipe_info)
{
	int	pipefd[2];

	if (!pipe_info)
		return (PROCESS_ERROR_PIPE);
	if (pipe(pipefd) == -1)
		return (PROCESS_ERROR_PIPE);
	pipe_info->read_fd = pipefd[0];
	pipe_info->write_fd = pipefd[1];
	return (PROCESS_SUCCESS);
}

static t_process_result	impl_close_pipe(t_pipe_info *pipe_info)
{
	if (!pipe_info)
		return (PROCESS_ERROR_PIPE);
	if (pipe_info->read_fd >= 0)
		close(pipe_info->read_fd);
	if (pipe_info->write_fd >= 0)
		close(pipe_info->write_fd);
	pipe_info->read_fd = -1;
	pipe_info->write_fd = -1;
	return (PROCESS_SUCCESS);
}

static t_process_result	impl_duplicate_fd(int old_fd, int new_fd)
{
	if (old_fd < 0 || new_fd < 0)
		return (PROCESS_ERROR_DUP);
	if (dup2(old_fd, new_fd) == -1)
		return (PROCESS_ERROR_DUP);
	return (PROCESS_SUCCESS);
}

static t_process_result	impl_close_fd(int fd)
{
	if (fd < 0)
		return (PROCESS_SUCCESS);
	if (close(fd) == -1)
		return (PROCESS_ERROR_DUP);
	return (PROCESS_SUCCESS);
}

static bool	impl_is_process_running(pid_t pid)
{
	int	status;

	if (pid <= 0)
		return (false);
	if (waitpid(pid, &status, WNOHANG) == 0)
		return (true);
	return (false);
}

static char	*impl_get_error_message(t_process_result result)
{
	if (result == PROCESS_ERROR_FORK)
		return (ft_strdup("Fork failed"));
	else if (result == PROCESS_ERROR_EXEC)
		return (ft_strdup("Exec failed"));
	else if (result == PROCESS_ERROR_WAIT)
		return (ft_strdup("Wait failed"));
	else if (result == PROCESS_ERROR_PIPE)
		return (ft_strdup("Pipe operation failed"));
	else if (result == PROCESS_ERROR_DUP)
		return (ft_strdup("File descriptor operation failed"));
	else if (result == PROCESS_ERROR_ACCESS)
		return (ft_strdup("Access denied"));
	return (ft_strdup("Unknown process error"));
}

t_process_service	*create_process_service(void)
{
	t_process_service	*service;

	service = malloc(sizeof(t_process_service));
	if (!service)
		return (NULL);
	service->fork_process = impl_fork_process;
	service->exec_command = impl_exec_command;
	service->wait_process = impl_wait_process;
	service->wait_all_processes = impl_wait_all_processes;
	service->create_pipe = impl_create_pipe;
	service->close_pipe = impl_close_pipe;
	service->duplicate_fd = impl_duplicate_fd;
	service->close_fd = impl_close_fd;
	service->is_process_running = impl_is_process_running;
	service->get_error_message = impl_get_error_message;
	return (service);
}

void	destroy_process_service(t_process_service *service)
{
	if (service)
		free(service);
}
