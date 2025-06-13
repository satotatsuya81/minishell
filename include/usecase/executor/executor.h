/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claude <claude@anthropic.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/01/06 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "domain/command.h"
# include "domain/env_initializer.h"

/* Exit status definitions */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_COMMAND_NOT_FOUND 127

/* Execution context structure */
typedef struct s_exec_context {
	t_env_var	**env;
	int			last_exit_status;
	int			should_exit;
	int			exit_code;
} t_exec_context;

/* Main executor functions */
int			execute_pipeline_list(t_pipeline *pipelines, t_exec_context *ctx);
int			execute_pipeline(t_pipeline *pipeline, t_exec_context *ctx);
int			execute_command_chain(t_cmd *cmds, t_exec_context *ctx);
int			execute_single_command(t_cmd *cmd, t_exec_context *ctx);

/* Command execution helpers */
int			is_builtin(const char *cmd);
int			execute_builtin(t_cmd *cmd, t_exec_context *ctx);
int			execute_external(t_cmd *cmd, t_exec_context *ctx);

/* Redirection handling */
int			setup_redirections(t_cmd_redirect *redirects);
void		restore_redirections(int saved_stdin, int saved_stdout);

/* Pipe handling */
int			execute_pipe_chain(t_cmd *cmds, t_exec_context *ctx);

/* Utility functions */
char		**env_to_envp(t_env_var *env);
char		*find_command_path(const char *cmd, t_env_var *env);
void		free_envp(char **envp);

/* Context management */
t_exec_context	*create_exec_context(t_env_var **env);
void			free_exec_context(t_exec_context *ctx);

#endif /* EXECUTOR_H */