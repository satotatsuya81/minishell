/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_complex_edge_cases.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/18 16:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "evaluation_test_suite.h"
#include <limits.h>

/* Test Suite: Variable Expansion */
Test(variables, basic_variable_expansion)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic variable expansion");
	
	t_test_result *result = execute_minishell_command("echo $USER", &ctx);
	cr_assert_not_null(result, "Variable expansion should work");
	cr_assert_neq(strlen(result->stdout_output), 0, "Should expand to actual value");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(variables, exit_status_expansion)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing $? expansion");
	
	t_test_result *result = execute_minishell_command("/bin/true\necho $?", &ctx);
	cr_assert_not_null(result, "$? expansion should work");
	cr_assert(strstr(result->stdout_output, "0") != NULL, "$? should show exit status");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(variables, variable_in_double_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing variable expansion in double quotes");
	
	t_test_result *result = execute_minishell_command("echo \"User: $USER\"", &ctx);
	cr_assert_not_null(result, "Variable in double quotes should expand");
	cr_assert(strstr(result->stdout_output, "User:") != NULL, "Should expand within quotes");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(variables, variable_in_single_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing variable NOT expanded in single quotes");
	
	t_test_result *result = execute_minishell_command("echo '$USER'", &ctx);
	cr_assert_not_null(result, "Variable in single quotes should work");
	cr_assert(strstr(result->stdout_output, "$USER") != NULL, 
		"Should NOT expand in single quotes");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Path Handling */
Test(paths, relative_path_execution)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing relative path command execution");
	
	t_test_result *result = execute_minishell_command("ls", &ctx);
	cr_assert_not_null(result, "Relative path should work");
	cr_assert_eq(result->exit_code, 0, "ls should succeed");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(paths, complex_relative_path)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing complex relative path");
	
	t_test_result *result = execute_minishell_command("cd ..\ncd ./minishell_tatsato\npwd", &ctx);
	cr_assert_not_null(result, "Complex relative path should work");
	cr_assert_eq(result->exit_code, 0, "Navigation should succeed");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(paths, path_environment_variable)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing PATH environment variable usage");
	
	t_test_result *result = execute_minishell_command("ls", &ctx);
	cr_assert_not_null(result, "PATH-based command should work");
	cr_assert_eq(result->exit_code, 0, "Command found via PATH should succeed");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(paths, unset_path_variable)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing behavior with unset PATH");
	
	t_test_result *result = execute_minishell_command("unset PATH\nls\necho $?", &ctx);
	cr_assert_not_null(result, "Unset PATH should be handled");
	cr_assert(strstr(result->stdout_output, "127") != NULL, 
		"Command not found should return 127");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(paths, multiple_path_directories)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing multiple directories in PATH");
	
	t_test_result *result = execute_minishell_command(
		"export PATH=/bin:/usr/bin\nls", &ctx);
	cr_assert_not_null(result, "Multiple PATH directories should work");
	cr_assert_eq(result->exit_code, 0, "Should find command in PATH");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Complex Scenarios */
Test(complex, long_command_line)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing long command line with many arguments");
	
	char long_command[2048] = "echo ";
	for (int i = 0; i < 50; i++)
	{
		strcat(long_command, "arg");
		char num[8];
		snprintf(num, sizeof(num), "%d ", i);
		strcat(long_command, num);
	}
	
	t_test_result *result = execute_minishell_command(long_command, &ctx);
	cr_assert_not_null(result, "Long command should work");
	cr_assert_eq(result->exit_code, 0, "Long command should succeed");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(complex, deeply_nested_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing complex nested quoting");
	
	t_test_result *result = execute_minishell_command(
		"echo \"single 'quotes' inside double\"", &ctx);
	cr_assert_not_null(result, "Nested quotes should work");
	cr_assert(strstr(result->stdout_output, "single 'quotes' inside double") != NULL,
		"Nested quotes should be handled correctly");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(complex, multiple_pipes_and_redirections)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing complex pipes and redirections");
	
	char *temp_file = "/tmp/minishell_complex_test";
	char command[512];
	snprintf(command, sizeof(command),
		"echo hello world | cat | cat > %s\ncat %s | cat", temp_file, temp_file);
	
	t_test_result *result = execute_minishell_command(command, &ctx);
	cr_assert_not_null(result, "Complex pipes and redirections should work");
	cr_assert(strstr(result->stdout_output, "hello world") != NULL,
		"Complex pipeline should work correctly");
	
	unlink(temp_file);
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(complex, command_substitution_like)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing complex variable scenarios");
	
	t_test_result *result = execute_minishell_command(
		"export VAR1=hello\nexport VAR2=world\necho $VAR1 $VAR2", &ctx);
	cr_assert_not_null(result, "Multiple variables should work");
	cr_assert(strstr(result->stdout_output, "hello world") != NULL,
		"Multiple variable expansion should work");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Edge Cases */
Test(edge_cases, extremely_long_input)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing extremely long input handling");
	
	char *long_input = malloc(4096);
	strcpy(long_input, "echo ");
	for (int i = 0; i < 1000; i++)
		strcat(long_input, "a");
	
	t_test_result *result = execute_minishell_command(long_input, &ctx);
	cr_assert_not_null(result, "Very long input should be handled");
	
	free(long_input);
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(edge_cases, null_bytes_in_input)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing input with special characters");
	
	t_test_result *result = execute_minishell_command("echo \"test\ttest\"", &ctx);
	cr_assert_not_null(result, "Special characters should be handled");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(edge_cases, maximum_argument_count)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing maximum number of arguments");
	
	char command[4096] = "echo ";
	for (int i = 0; i < 100; i++)
	{
		char arg[16];
		snprintf(arg, sizeof(arg), "arg%d ", i);
		strcat(command, arg);
	}
	
	t_test_result *result = execute_minishell_command(command, &ctx);
	cr_assert_not_null(result, "Many arguments should be handled");
	cr_assert_eq(result->exit_code, 0, "Should succeed with many arguments");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(edge_cases, invalid_command_characters)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing invalid command with special characters");
	
	t_test_result *result = execute_minishell_command("invalidcommand123", &ctx);
	cr_assert_not_null(result, "Invalid command should be handled");
	cr_assert_neq(result->exit_code, 0, "Invalid command should fail");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Error Handling */
Test(error_handling, syntax_errors)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing syntax error handling");
	
	t_test_result *result = execute_minishell_command("ls |", &ctx);
	cr_assert_not_null(result, "Syntax errors should be handled");
	/* Should show error and not crash */
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(error_handling, permission_denied)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing permission denied handling");
	
	t_test_result *result = execute_minishell_command("cat /etc/shadow 2>/dev/null", &ctx);
	cr_assert_not_null(result, "Permission errors should be handled");
	/* Should handle permission errors gracefully */
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(error_handling, file_not_found)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing file not found handling");
	
	t_test_result *result = execute_minishell_command("cat /nonexistent/file", &ctx);
	cr_assert_not_null(result, "File not found should be handled");
	cr_assert_neq(result->exit_code, 0, "Should return error code");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Memory and Performance */
Test(performance, rapid_command_execution, .timeout = 10)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing rapid command execution performance");
	
	for (int i = 0; i < 10; i++)
	{
		t_test_result *result = execute_minishell_command("echo test", &ctx);
		cr_assert_not_null(result, "Rapid execution should work");
		cr_assert_eq(result->exit_code, 0, "Each command should succeed");
		free_test_result(result);
	}
	
	cleanup_test_environment(&ctx);
}

Test(performance, memory_stress_test, .timeout = 30)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing memory usage under stress");
	
	/* Create commands that use significant memory */
	char command[1024];
	for (int i = 0; i < 5; i++)
	{
		snprintf(command, sizeof(command), 
			"echo test | cat | cat | cat | cat");
		
		t_test_result *result = execute_minishell_command(command, &ctx);
		cr_assert_not_null(result, "Memory stress test should work");
		free_test_result(result);
	}
	
	cleanup_test_environment(&ctx);
}