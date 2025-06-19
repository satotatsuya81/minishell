/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_echo_criterion_tdt.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/17 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include "usecase/builtin/builtin_commands.h"
#include "domain/env_variable.h"

/* ************************************************************************** */
/*                          Expected Results Definition                       */
/* ************************************************************************** */

// Expected results for echo command operations
typedef struct s_expected_echo {
	int				command_success;		// 1: コマンド成功期待
	const char		*expected_output;		// 期待出力内容
	int				newline_expected;		// 1: 改行期待
	int				output_length;			// 期待出力長
} t_expected_echo;

/* ************************************************************************** */
/*                          Expected Data Definitions                        */
/* ************************************************************************** */

// Simple string output
static const t_expected_echo simple_string_expected = {
	1,			// command_success
	"hello world",	// expected_output
	1,			// newline_expected
	11			// output_length
};

// Multiple arguments
static const t_expected_echo multiple_args_expected = {
	1,			// command_success
	"hello world test",	// expected_output
	1,			// newline_expected
	17			// output_length
};

// Echo with -n flag (no newline)
static const t_expected_echo no_newline_expected = {
	1,			// command_success
	"hello",	// expected_output
	0,			// newline_expected
	5			// output_length
};

// Empty echo (just newline)
static const t_expected_echo empty_echo_expected = {
	1,			// command_success
	"",			// expected_output
	1,			// newline_expected
	0			// output_length
};

// Echo with special characters
static const t_expected_echo special_chars_expected = {
	1,			// command_success
	"hello\tworld\ntest",	// expected_output
	1,			// newline_expected
	17			// output_length
};

/* ************************************************************************** */
/*                          Echo Test Case Structure                         */
/* ************************************************************************** */

typedef struct s_echo_test_case {
	const char			*input_description;
	char				**argv;
	int					argc;
	int					expected_success;
	const t_expected_echo	*expected_result;
	const char			*test_name;
} t_echo_test_case;

/* ************************************************************************** */
/*                          Test Implementation Functions                    */
/* ************************************************************************** */

static char	**create_echo_argv(int argc, ...)
{
	va_list	args;
	char	**argv;
	int		i;

	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	
	va_start(args, argc);
	for (i = 0; i < argc; i++)
	{
		char *arg = va_arg(args, char *);
		if (arg)
			argv[i] = strdup(arg);
		else
			argv[i] = NULL;
	}
	va_end(args);
	argv[argc] = NULL;
	return (argv);
}

static void	free_echo_argv(char **argv)
{
	int	i;

	if (!argv)
		return;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static int	capture_echo_output(char **argv, char *output_buffer, size_t buffer_size)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		status;
	ssize_t	bytes_read;

	if (pipe(pipe_fds) == -1)
		return (0);

	pid = fork();
	if (pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (0);
	}

	if (pid == 0)  // 子プロセス
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		
		// echo実行
		ft_echo(argv, NULL);
		exit(0);
	}
	else  // 親プロセス
	{
		close(pipe_fds[1]);
		bytes_read = read(pipe_fds[0], output_buffer, buffer_size - 1);
		if (bytes_read > 0)
			output_buffer[bytes_read] = '\0';
		else
			output_buffer[0] = '\0';
		
		close(pipe_fds[0]);
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status) == 0);
	}
}

static int	verify_echo_operation(char **argv, const t_expected_echo *expected)
{
	char	output_buffer[1024];
	int		success;
	size_t	output_len;
	char	*newline_pos;

	// echo出力をキャプチャ
	success = capture_echo_output(argv, output_buffer, sizeof(output_buffer));
	
	if (success != expected->command_success)
		return (0);

	if (!expected->command_success)
		return (1);  // エラーケースは成功

	// 出力内容確認
	output_len = strlen(output_buffer);
	
	// 改行文字の処理
	newline_pos = strrchr(output_buffer, '\n');
	if (newline_pos && !expected->newline_expected)
		return (0);  // 改行があるのに期待されていない
	
	if (!newline_pos && expected->newline_expected)
		return (0);  // 改行がないのに期待されている

	// 改行を除いた内容で比較
	if (newline_pos)
		*newline_pos = '\0';

	if (strcmp(output_buffer, expected->expected_output) != 0)
		return (0);

	return (1);
}

/* ************************************************************************** */
/*                          Test Data Table (TDT)                            */
/* ************************************************************************** */

// テストケースのargv配列を事前定義
static char *simple_argv[] = {"echo", "hello", "world", NULL};
static char *multiple_argv[] = {"echo", "hello", "world", "test", NULL};
static char *no_newline_argv[] = {"echo", "-n", "hello", NULL};
static char *empty_argv[] = {"echo", NULL};
static char *special_argv[] = {"echo", "hello\tworld\ntest", NULL};

static const t_echo_test_case echo_test_cases[] = {
	{
		"echo simple string 'hello world'",
		simple_argv,
		3,
		1,
		&simple_string_expected,
		"simple_string"
	},
	{
		"echo multiple arguments",
		multiple_argv,
		4,
		1,
		&multiple_args_expected,
		"multiple_arguments"
	},
	{
		"echo with -n flag (no newline)",
		no_newline_argv,
		3,
		1,
		&no_newline_expected,
		"no_newline"
	},
	{
		"echo with no arguments (empty)",
		empty_argv,
		1,
		1,
		&empty_echo_expected,
		"empty_echo"
	},
	{
		"echo with special characters",
		special_argv,
		2,
		1,
		&special_chars_expected,
		"special_characters"
	}
};

/* ************************************************************************** */
/*                          Test Runner Implementation                       */
/* ************************************************************************** */

Test(builtin_echo, table_driven_tests)
{
	size_t			case_count;
	size_t			i;
	int				result;
	int				passed;
	const t_echo_test_case	*test_case;

	case_count = sizeof(echo_test_cases) / sizeof(echo_test_cases[0]);
	passed = 0;

	cr_log_info("=== Echo Criterion TDT Test Suite ===");
	
	for (i = 0; i < case_count; i++)
	{
		test_case = &echo_test_cases[i];
		cr_log_info("Running test %zu: %s", i, test_case->input_description);

		// echo操作実行・検証
		result = verify_echo_operation(test_case->argv, test_case->expected_result);

		cr_assert(result, "Test '%s' failed: echo operation verification failed",
				test_case->test_name);

		if (result)
		{
			cr_log_info("✅ Test %zu passed: %s", i, test_case->test_name);
			passed++;
		}
		else
		{
			cr_log_error("❌ Test %zu failed: %s", i, test_case->test_name);
		}
	}

	cr_log_info("=== Echo Criterion TDT Summary ===");
	cr_log_info("Passed: %d/%zu", passed, case_count);
	
	if (passed == (int)case_count)
		cr_log_info("🎉 All Echo Criterion TDT tests passed!");
	else
		cr_log_warn("⚠️  Some Echo Criterion TDT tests failed.");
}