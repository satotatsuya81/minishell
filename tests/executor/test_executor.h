/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_executor.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/14 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_EXECUTOR_H
# define TEST_EXECUTOR_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <unistd.h>
# include <sys/wait.h>
# include "usecase/executor/executor.h"
# include "domain/command.h"
# include "domain/env_initializer.h"

/* Test utilities */
t_cmd			*create_test_cmd(char **argv);
t_cmd_redirect	*create_test_redirect(t_redirect_type type, const char *file);
t_exec_context	*create_test_context(void);
void			cleanup_test_files(void);

/* Builtin executor tests */
void			test_builtin_echo(void);
void			test_builtin_pwd(void);
void			test_builtin_env(void);
void			test_builtin_cd(void);
void			test_builtin_exit(void);

/* External executor tests */
void			test_external_ls(void);
void			test_external_cat(void);
void			test_external_invalid_command(void);

/* Redirection tests */
void			test_output_redirection(void);
void			test_input_redirection(void);
void			test_append_redirection(void);

/* Pipeline tests */
void			test_simple_pipe(void);
void			test_multiple_pipes(void);

/* Integration tests */
void			test_command_with_redirection(void);
void			test_pipe_with_redirection(void);

#endif