/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation_test_suite.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/18 16:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVALUATION_TEST_SUITE_H
# define EVALUATION_TEST_SUITE_H

# include <criterion/criterion.h>
# include <criterion/redirect.h>
# include <criterion/logging.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>

/* Test utilities */
typedef struct s_test_context
{
	char			**envp;
	int				stdin_backup;
	int				stdout_backup;
	int				stderr_backup;
	char			*temp_file;
	pid_t			child_pid;
}	t_test_context;

/* Test result structure */
typedef struct s_test_result
{
	int				exit_code;
	char			*stdout_output;
	char			*stderr_output;
	int				signal_received;
}	t_test_result;

/* Test setup and teardown */
void			setup_test_environment(t_test_context *ctx);
void			cleanup_test_environment(t_test_context *ctx);
t_test_result	*execute_minishell_command(const char *command, t_test_context *ctx);
void			free_test_result(t_test_result *result);

/* Test categories based on 42 evaluation guide */
void			test_compilation_requirements(void);
void			test_simple_commands(void);
void			test_global_variables(void);
void			test_arguments_and_history(void);
void			test_echo_builtin(void);
void			test_exit_builtin(void);
void			test_return_values(void);
void			test_signal_handling(void);
void			test_double_quotes(void);
void			test_single_quotes(void);
void			test_env_builtin(void);
void			test_export_builtin(void);
void			test_unset_builtin(void);
void			test_cd_builtin(void);
void			test_pwd_builtin(void);
void			test_relative_paths(void);
void			test_environment_path(void);
void			test_redirections(void);
void			test_pipes(void);
void			test_complex_scenarios(void);
void			test_environment_variables(void);

/* Edge case and error handling tests */
void			test_memory_leaks(void);
void			test_edge_cases(void);
void			test_error_handling(void);
void			test_performance_stress(void);

/* Signal testing utilities */
void			test_sigint_behavior(void);
void			test_sigquit_behavior(void);
void			test_eof_behavior(void);

#endif