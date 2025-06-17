/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cd_criterion_tdt.c                           :+:      :+:    :+:   */
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
#include "usecase/builtin/builtin_commands.h"
#include "domain/env_variable.h"
#include "usecase/env/env_manager.h"

/* ************************************************************************** */
/*                          Expected Results Definition                       */
/* ************************************************************************** */

// Expected results for cd command operations
typedef struct s_expected_cd {
	int				command_success;		// 1: コマンド成功期待
	const char		*expected_final_dir;	// 期待最終ディレクトリ
	int				pwd_should_change;		// 1: PWD変更期待
	int				oldpwd_should_change;	// 1: OLDPWD変更期待
	const char		*expected_error_msg;	// 期待エラーメッセージ
} t_expected_cd;

/* ************************************************************************** */
/*                          Expected Data Definitions                        */
/* ************************************************************************** */

// Successful absolute path change
static const t_expected_cd absolute_path_expected = {
	1,			// command_success
	"/tmp",		// expected_final_dir
	1,			// pwd_should_change
	1,			// oldpwd_should_change
	NULL		// expected_error_msg
};

// Successful relative path change
static const t_expected_cd relative_path_expected = {
	1,			// command_success
	NULL,		// expected_final_dir (計算される)
	1,			// pwd_should_change
	1,			// oldpwd_should_change
	NULL		// expected_error_msg
};

// cd to HOME directory
static const t_expected_cd home_directory_expected = {
	1,			// command_success
	"/home/test",	// expected_final_dir
	1,			// pwd_should_change
	1,			// oldpwd_should_change
	NULL		// expected_error_msg
};

// cd to nonexistent directory (error case)
static const t_expected_cd nonexistent_dir_expected = {
	0,			// command_success (失敗期待)
	NULL,		// expected_final_dir (変更されない)
	0,			// pwd_should_change
	0,			// oldpwd_should_change
	"cd: no such file or directory"	// expected_error_msg
};

// cd with NULL argument (should go to HOME)
static const t_expected_cd null_argument_expected = {
	1,			// command_success
	"/home/test",	// expected_final_dir
	1,			// pwd_should_change
	1,			// oldpwd_should_change
	NULL		// expected_error_msg
};

/* ************************************************************************** */
/*                          CD Test Case Structure                           */
/* ************************************************************************** */

typedef struct s_cd_test_case {
	const char			*input_description;
	char				**argv;
	const char			*initial_directory;
	const char			*home_value;
	int					expected_success;
	const t_expected_cd	*expected_result;
	const char			*test_name;
} t_cd_test_case;

/* ************************************************************************** */
/*                          Test Implementation Functions                    */
/* ************************************************************************** */

static char	**create_test_argv(const char *arg)
{
	char	**argv;

	argv = malloc(sizeof(char *) * 3);
	if (!argv)
		return (NULL);
	argv[0] = strdup("cd");
	if (arg)
		argv[1] = strdup(arg);
	else
		argv[1] = NULL;
	argv[2] = NULL;
	return (argv);
}

static void	free_test_argv(char **argv)
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

static t_env_var	*create_test_env(const char *home_value)
{
	char	**envp;
	char	*home_entry;
	t_env_var	*env;

	home_entry = malloc(strlen("HOME=") + strlen(home_value) + 1);
	if (!home_entry)
		return (NULL);
	strcpy(home_entry, "HOME=");
	strcat(home_entry, home_value);
	
	envp = malloc(sizeof(char *) * 4);
	if (!envp)
	{
		free(home_entry);
		return (NULL);
	}
	envp[0] = home_entry;
	envp[1] = strdup("PWD=/current/test");
	envp[2] = strdup("OLDPWD=/previous/test");
	envp[3] = NULL;

	env = env_create_from_envp(envp);
	
	free(home_entry);
	free(envp[1]);
	free(envp[2]);
	free(envp);
	
	return (env);
}

static int	verify_cd_operation(char **argv, t_env_var **env,
		const char *initial_dir, const t_expected_cd *expected)
{
	char	*original_dir;
	char	*current_dir;
	int		result;

	// 現在ディレクトリ保存
	original_dir = getcwd(NULL, 0);
	if (!original_dir)
		return (0);

	// 初期ディレクトリ設定
	if (initial_dir && chdir(initial_dir) != 0)
	{
		free(original_dir);
		return (0);
	}

	// cd コマンド実行
	result = ft_cd(argv, env);

	// 結果検証
	if ((result == 0) != expected->command_success)
	{
		chdir(original_dir);
		free(original_dir);
		return (0);
	}

	// 成功時のディレクトリ確認
	if (expected->command_success && expected->expected_final_dir)
	{
		current_dir = getcwd(NULL, 0);
		if (!current_dir || strcmp(current_dir, expected->expected_final_dir) != 0)
		{
			if (current_dir)
				free(current_dir);
			chdir(original_dir);
			free(original_dir);
			return (0);
		}
		free(current_dir);
	}

	// 元のディレクトリに戻す
	chdir(original_dir);
	free(original_dir);
	return (1);
}

/* ************************************************************************** */
/*                          Test Data Table (TDT)                            */
/* ************************************************************************** */

static const t_cd_test_case cd_test_cases[] = {
	{
		"cd to absolute path /tmp",
		NULL,  // argvは動的生成
		"/current/test",
		"/home/test",
		1,
		&absolute_path_expected,
		"absolute_path"
	},
	{
		"cd with relative path ..",
		NULL,  // argvは動的生成
		"/current/test/subdir",
		"/home/test",
		1,
		&relative_path_expected,
		"relative_path"
	},
	{
		"cd with no arguments (should go to HOME)",
		NULL,  // argvは動的生成
		"/current/test",
		"/home/test",
		1,
		&home_directory_expected,
		"home_directory"
	},
	{
		"cd to nonexistent directory",
		NULL,  // argvは動的生成
		"/current/test",
		"/home/test",
		0,
		&nonexistent_dir_expected,
		"nonexistent_directory"
	},
	{
		"cd with NULL argument (explicit HOME test)",
		NULL,  // argvは動的生成
		"/current/test",
		"/home/test",
		1,
		&null_argument_expected,
		"null_argument"
	}
};

/* ************************************************************************** */
/*                          Test Runner Implementation                       */
/* ************************************************************************** */

Test(builtin_cd, table_driven_tests)
{
	size_t			case_count;
	size_t			i;
	t_env_var		*test_env;
	char			**test_argv;
	int				result;
	int				passed;
	const t_cd_test_case	*test_case;

	case_count = sizeof(cd_test_cases) / sizeof(cd_test_cases[0]);
	passed = 0;

	cr_log_info("=== CD Criterion TDT Test Suite ===");
	
	for (i = 0; i < case_count; i++)
	{
		test_case = &cd_test_cases[i];
		cr_log_info("Running test %zu: %s", i, test_case->input_description);

		// テスト環境作成
		test_env = create_test_env(test_case->home_value);
		cr_assert_not_null(test_env, "Failed to create test environment");

		// argv生成（テストケースに応じて）
		if (i == 0)  // absolute_path
			test_argv = create_test_argv("/tmp");
		else if (i == 1)  // relative_path
			test_argv = create_test_argv("..");
		else if (i == 2 || i == 4)  // home_directory, null_argument
			test_argv = create_test_argv(NULL);
		else if (i == 3)  // nonexistent_directory
			test_argv = create_test_argv("/nonexistent/path");
		else
			test_argv = create_test_argv(NULL);

		cr_assert_not_null(test_argv, "Failed to create test argv");

		// cd操作実行・検証
		result = verify_cd_operation(test_argv, &test_env,
				test_case->initial_directory, test_case->expected_result);

		cr_assert(result, "Test '%s' failed: cd operation verification failed",
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

		// クリーンアップ
		free_test_argv(test_argv);
		env_free(test_env);
	}

	cr_log_info("=== CD Criterion TDT Summary ===");
	cr_log_info("Passed: %d/%zu", passed, case_count);
	
	if (passed == (int)case_count)
		cr_log_info("🎉 All CD Criterion TDT tests passed!");
	else
		cr_log_warn("⚠️  Some CD Criterion TDT tests failed.");
}