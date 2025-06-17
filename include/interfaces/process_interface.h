/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_interface.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/16 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_INTERFACE_H
# define PROCESS_INTERFACE_H

# include <sys/types.h>
# include <stdbool.h>

/* Process operation results */
typedef enum e_process_result {
	PROCESS_SUCCESS,
	PROCESS_ERROR_FORK,
	PROCESS_ERROR_EXEC,
	PROCESS_ERROR_WAIT,
	PROCESS_ERROR_PIPE,
	PROCESS_ERROR_DUP,
	PROCESS_ERROR_ACCESS
} t_process_result;

/* Process information structure */
typedef struct s_process_info {
	pid_t	pid;
	int		exit_status;
	bool	is_running;
} t_process_info;

/* Pipe structure */
typedef struct s_pipe_info {
	int	read_fd;
	int	write_fd;
} t_pipe_info;

/* Process management service interface */
typedef struct s_process_service {
	/* Process creation and execution */
	t_process_result	(*fork_process)(pid_t *pid);
	t_process_result	(*exec_command)(const char *path, char **argv, 
							char **envp);
	t_process_result	(*wait_process)(pid_t pid, int *exit_status);
	t_process_result	(*wait_all_processes)(pid_t *pids, int count, 
							int *exit_statuses);
	
	/* Pipe management */
	t_process_result	(*create_pipe)(t_pipe_info *pipe_info);
	t_process_result	(*close_pipe)(t_pipe_info *pipe_info);
	t_process_result	(*duplicate_fd)(int old_fd, int new_fd);
	t_process_result	(*close_fd)(int fd);
	
	/* Process information */
	bool				(*is_process_running)(pid_t pid);
	char				*(*get_error_message)(t_process_result result);
} t_process_service;

/* Service factory functions */
t_process_service	*create_process_service(void);
void				destroy_process_service(t_process_service *service);

#endif