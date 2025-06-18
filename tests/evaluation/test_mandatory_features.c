/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mandatory_features.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/18 16:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "evaluation_test_suite.h"

/* Test Suite: Compilation Requirements */
Test(compilation, makefile_flags)
{
	cr_log_info("Testing compilation flags compliance");
	
	/* Test that makefile uses required flags */
	int result = system("make -n 2>&1 | grep -q '\\-Wall \\-Wextra \\-Werror'");
	cr_assert_eq(result, 0, "Makefile must use -Wall -Wextra -Werror flags");
}

Test(compilation, clean_compile)
{
	cr_log_info("Testing clean compilation without errors");
	
	/* Test clean compilation */
	int result = system("make clean > /dev/null 2>&1 && make > /dev/null 2>&1");
	cr_assert_eq(result, 0, "Project must compile without errors");
}

Test(compilation, no_relink)
{
	cr_log_info("Testing that makefile doesn't relink unnecessarily");
	
	/* Test no unnecessary relinking */
	system("make > /dev/null 2>&1");
	int result = system("make 2>&1 | grep -q 'Nothing to be done\\|up to date'");
	cr_assert_eq(result, 0, "Makefile must not relink when not necessary");
}

/* Test Suite: Simple Commands & Global Variables */
Test(simple_commands, absolute_path_command)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing absolute path command execution");
	
	t_test_result *result = execute_minishell_command("/bin/ls", &ctx);
	cr_assert_not_null(result, "Command execution should not fail");
	cr_assert_eq(result->exit_code, 0, "Absolute path command should succeed");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(simple_commands, empty_command)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing empty command handling");
	
	t_test_result *result = execute_minishell_command("", &ctx);
	cr_assert_not_null(result, "Empty command should be handled gracefully");
	cr_assert_eq(result->exit_code, 0, "Empty command should not cause errors");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(simple_commands, spaces_and_tabs)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing spaces and tabs only input");
	
	t_test_result *result = execute_minishell_command("   \t  \t ", &ctx);
	cr_assert_not_null(result, "Whitespace-only input should be handled");
	cr_assert_eq(result->exit_code, 0, "Whitespace should not cause errors");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Arguments & History */
Test(arguments, command_with_arguments)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing command with arguments");
	
	t_test_result *result = execute_minishell_command("/bin/echo hello world", &ctx);
	cr_assert_not_null(result, "Command with arguments should execute");
	cr_assert_str_neq(result->stdout_output, NULL, "Command should produce output");
	cr_assert(strstr(result->stdout_output, "hello world") != NULL, 
		"Output should contain the arguments");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(arguments, multiple_arguments)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing command with multiple arguments");
	
	t_test_result *result = execute_minishell_command("/bin/echo arg1 arg2 arg3", &ctx);
	cr_assert_not_null(result, "Command with multiple arguments should execute");
	cr_assert(strstr(result->stdout_output, "arg1 arg2 arg3") != NULL,
		"All arguments should be passed correctly");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Echo Builtin */
Test(echo_builtin, basic_echo)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic echo functionality");
	
	t_test_result *result = execute_minishell_command("echo hello", &ctx);
	cr_assert_not_null(result, "Echo command should execute");
	cr_assert(strstr(result->stdout_output, "hello") != NULL,
		"Echo should output the text");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(echo_builtin, echo_with_n_flag)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing echo with -n flag");
	
	t_test_result *result = execute_minishell_command("echo -n hello", &ctx);
	cr_assert_not_null(result, "Echo -n should execute");
	cr_assert(strstr(result->stdout_output, "hello") != NULL,
		"Echo -n should output text without newline");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(echo_builtin, echo_no_arguments)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing echo without arguments");
	
	t_test_result *result = execute_minishell_command("echo", &ctx);
	cr_assert_not_null(result, "Echo without arguments should execute");
	cr_assert_eq(result->exit_code, 0, "Echo should succeed even without arguments");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Exit Builtin */
Test(exit_builtin, exit_no_arguments)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing exit without arguments");
	
	t_test_result *result = execute_minishell_command("exit", &ctx);
	cr_assert_not_null(result, "Exit command should execute");
	cr_assert_eq(result->exit_code, 0, "Exit without arguments should return 0");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(exit_builtin, exit_with_valid_code)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing exit with valid exit code");
	
	t_test_result *result = execute_minishell_command("exit 42", &ctx);
	cr_assert_not_null(result, "Exit with code should execute");
	cr_assert_eq(result->exit_code, 42, "Exit should return specified code");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(exit_builtin, exit_with_invalid_code)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing exit with invalid argument");
	
	t_test_result *result = execute_minishell_command("exit invalid", &ctx);
	cr_assert_not_null(result, "Exit with invalid argument should be handled");
	/* Should output error but still exit */
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Return Values */
Test(return_values, successful_command_return)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing return value of successful command");
	
	t_test_result *result = execute_minishell_command("/bin/true\necho $?", &ctx);
	cr_assert_not_null(result, "Command sequence should execute");
	cr_assert(strstr(result->stdout_output, "0") != NULL,
		"Successful command should return 0");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(return_values, failed_command_return)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing return value of failed command");
	
	t_test_result *result = execute_minishell_command("/bin/false\necho $?", &ctx);
	cr_assert_not_null(result, "Command sequence should execute");
	cr_assert(strstr(result->stdout_output, "1") != NULL,
		"Failed command should return 1");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(return_values, nonexistent_command_return)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing return value of nonexistent command");
	
	t_test_result *result = execute_minishell_command("/bin/nonexistent\necho $?", &ctx);
	cr_assert_not_null(result, "Command sequence should execute");
	cr_assert(strstr(result->stdout_output, "127") != NULL,
		"Nonexistent command should return 127");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(return_values, arithmetic_with_return_value)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing arithmetic with return value");
	
	t_test_result *result = execute_minishell_command("/bin/false\nexpr $? + $?", &ctx);
	cr_assert_not_null(result, "Arithmetic with $? should work");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}