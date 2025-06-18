/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato            #+#    #+#             */
/*   Updated: 2025/06/16 07:37:44 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "domain/command.h"
# include "domain/env_variable.h"
# include "interfaces/io_interface.h"
# include "interfaces/output_interface.h"
# include <sys/wait.h>

/* Exit status definitions */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_COMMAND_NOT_FOUND 127

/* Execution context structure */
typedef struct s_exec_context
{
	t_env_var		**env;
	t_io_service	*io_service;
	t_output_service *output_service;
	int				last_exit_status;
	int				should_exit;
	int				exit_code;
}	t_exec_context;

/* Pipe execution parameters */
typedef struct s_pipe_params
{
	t_cmd			*cmd;
	int				input_fd;
	int				output_fd;
	t_exec_context	*ctx;
	int				*pipefd;
	int				cmd_count;
}	t_pipe_params;

/* Command loop execution parameters */
typedef struct s_cmd_loop_params
{
	t_cmd			*cmds;
	int				*pipefd;
	pid_t			*pids;
	t_exec_context	*ctx;
	int				cmd_count;
}	t_cmd_loop_params;

/* Main executor functions */
int			execute_pipeline_list(t_pipeline *pipelines, t_exec_context *ctx);
int			execute_pipeline(t_pipeline *pipeline, t_exec_context *ctx);
int			execute_command_chain(t_cmd *cmds, t_exec_context *ctx);
int			execute_single_command(t_cmd *cmd, t_exec_context *ctx);

/* Pipe utility functions */
int			count_commands(t_cmd *cmds);
int			allocate_pipe_resources(t_cmd *cmds, int **pipefd, pid_t **pids);
int			create_pipes(int *pipefd, int cmd_count);
void		cleanup_pipes(int *pipefd, int cmd_count);
int			wait_for_children(pid_t *pids, int cmd_count);
int			execute_commands_loop(t_cmd_loop_params *params);
int			execute_pipe_command(t_pipe_params *params);

/* Command execution helpers */
int			is_builtin(const char *cmd);
int			execute_builtin(t_cmd *cmd, t_exec_context *ctx);
int			execute_external(t_cmd *cmd, t_exec_context *ctx);

/* Environment and command path utilities */
char		**env_to_envp(t_env_var *env);
void		free_envp(char **envp);
char		*find_command_path(const char *cmd, t_env_var *env);

/* Execution context management */
t_exec_context	*create_exec_context(t_env_var **env, 
					t_io_service *io, t_output_service *out);
void			free_exec_context(t_exec_context *ctx);

/* Redirection handling */
int			setup_redirections(t_cmd_redirect *redirects);
void		restore_redirections(int saved_stdin, int saved_stdout);
int			handle_input_redirect(const char *filename);
int			handle_output_redirect(const char *filename);
int			handle_append_redirect(const char *filename);
int			handle_heredoc_redirect(const char *delimiter);

/* Pipe handling */
int			execute_pipe_chain(t_cmd *cmds, t_exec_context *ctx);

/* Variable expansion */
char		*expand_variables(const char *str, t_exec_context *ctx);
void		expand_command_variables(t_cmd *cmd, t_exec_context *ctx);

/* Note: Utility functions declared above in "Environment and command path utilities" section */

#endif /* EXECUTOR_H */
