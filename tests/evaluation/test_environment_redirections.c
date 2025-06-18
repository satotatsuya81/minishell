/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_environment_redirections.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/18 16:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "evaluation_test_suite.h"
#include <fcntl.h>

/* Test Suite: Environment Variables (env) */
Test(env_builtin, basic_env_display)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic env command");
	
	t_test_result *result = execute_minishell_command("env", &ctx);
	cr_assert_not_null(result, "env command should execute");
	cr_assert_eq(result->exit_code, 0, "env should succeed");
	cr_assert_not_null(result->stdout_output, "env should produce output");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(env_builtin, env_shows_current_variables)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing env shows current environment variables");
	
	t_test_result *result = execute_minishell_command("env", &ctx);
	cr_assert_not_null(result, "env command should execute");
	/* Should show standard environment variables like PATH, USER, etc. */
	cr_assert(strstr(result->stdout_output, "PATH=") != NULL ||
			  strstr(result->stdout_output, "USER=") != NULL,
		"env should show environment variables");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Export Builtin */
Test(export_builtin, basic_export)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic export functionality");
	
	t_test_result *result = execute_minishell_command("export TEST_VAR=hello\nenv | grep TEST_VAR", &ctx);
	cr_assert_not_null(result, "export command should execute");
	cr_assert(strstr(result->stdout_output, "TEST_VAR=hello") != NULL,
		"exported variable should appear in env");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(export_builtin, export_display_all)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing export without arguments displays all variables");
	
	t_test_result *result = execute_minishell_command("export", &ctx);
	cr_assert_not_null(result, "export without args should execute");
	cr_assert_eq(result->exit_code, 0, "export should succeed");
	cr_assert_not_null(result->stdout_output, "export should show variables");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(export_builtin, export_replace_existing)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing export replaces existing variables");
	
	t_test_result *result = execute_minishell_command(
		"export TEST_VAR=first\nexport TEST_VAR=second\nenv | grep TEST_VAR", &ctx);
	cr_assert_not_null(result, "export replacement should work");
	cr_assert(strstr(result->stdout_output, "TEST_VAR=second") != NULL,
		"export should replace existing variable");
	cr_assert(strstr(result->stdout_output, "TEST_VAR=first") == NULL,
		"old value should be replaced");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(export_builtin, export_new_variable)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing export creates new variables");
	
	t_test_result *result = execute_minishell_command(
		"export NEW_VAR=value\nenv | grep NEW_VAR", &ctx);
	cr_assert_not_null(result, "export new variable should work");
	cr_assert(strstr(result->stdout_output, "NEW_VAR=value") != NULL,
		"new variable should be created");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Unset Builtin */
Test(unset_builtin, basic_unset)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic unset functionality");
	
	t_test_result *result = execute_minishell_command(
		"export TEST_VAR=hello\nunset TEST_VAR\nenv | grep TEST_VAR", &ctx);
	cr_assert_not_null(result, "unset command should execute");
	cr_assert(strstr(result->stdout_output, "TEST_VAR=hello") == NULL,
		"unset should remove the variable");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(unset_builtin, unset_multiple_variables)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing unset with multiple variables");
	
	t_test_result *result = execute_minishell_command(
		"export VAR1=a\nexport VAR2=b\nunset VAR1 VAR2\nenv | grep VAR", &ctx);
	cr_assert_not_null(result, "unset multiple should work");
	cr_assert(strstr(result->stdout_output, "VAR1=a") == NULL &&
			  strstr(result->stdout_output, "VAR2=b") == NULL,
		"unset should remove multiple variables");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(unset_builtin, unset_nonexistent)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing unset with nonexistent variable");
	
	t_test_result *result = execute_minishell_command("unset NONEXISTENT_VAR", &ctx);
	cr_assert_not_null(result, "unset nonexistent should work");
	cr_assert_eq(result->exit_code, 0, "unset nonexistent should not error");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: CD and PWD Builtins */
Test(cd_builtin, basic_cd)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic cd functionality");
	
	t_test_result *result = execute_minishell_command("cd ..\npwd", &ctx);
	cr_assert_not_null(result, "cd command should execute");
	cr_assert_eq(result->exit_code, 0, "cd should succeed");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(cd_builtin, cd_relative_path)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing cd with relative path");
	
	t_test_result *result = execute_minishell_command("cd .\npwd", &ctx);
	cr_assert_not_null(result, "cd . should work");
	cr_assert_eq(result->exit_code, 0, "cd . should succeed");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(cd_builtin, cd_nonexistent_directory)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing cd with nonexistent directory");
	
	t_test_result *result = execute_minishell_command("cd /nonexistent\necho $?", &ctx);
	cr_assert_not_null(result, "cd to nonexistent should be handled");
	cr_assert(strstr(result->stdout_output, "1") != NULL,
		"cd failure should return error code 1");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(pwd_builtin, basic_pwd)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic pwd functionality");
	
	t_test_result *result = execute_minishell_command("pwd", &ctx);
	cr_assert_not_null(result, "pwd command should execute");
	cr_assert_eq(result->exit_code, 0, "pwd should succeed");
	cr_assert_not_null(result->stdout_output, "pwd should show current directory");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Redirections */
Test(redirections, output_redirection)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing output redirection");
	
	char *temp_file = "/tmp/minishell_test_output";
	char command[256];
	snprintf(command, sizeof(command), "echo hello > %s\ncat %s", temp_file, temp_file);
	
	t_test_result *result = execute_minishell_command(command, &ctx);
	cr_assert_not_null(result, "output redirection should work");
	cr_assert(strstr(result->stdout_output, "hello") != NULL,
		"redirected output should be readable");
	
	unlink(temp_file); /* Clean up */
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(redirections, input_redirection)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing input redirection");
	
	/* Create a test file first */
	char *temp_file = "/tmp/minishell_test_input";
	int fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(fd, "test input\n", 11);
	close(fd);
	
	char command[256];
	snprintf(command, sizeof(command), "cat < %s", temp_file);
	
	t_test_result *result = execute_minishell_command(command, &ctx);
	cr_assert_not_null(result, "input redirection should work");
	cr_assert(strstr(result->stdout_output, "test input") != NULL,
		"input redirection should read from file");
	
	unlink(temp_file); /* Clean up */
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(redirections, append_redirection)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing append redirection");
	
	char *temp_file = "/tmp/minishell_test_append";
	char command[512];
	snprintf(command, sizeof(command), 
		"echo first > %s\necho second >> %s\ncat %s", 
		temp_file, temp_file, temp_file);
	
	t_test_result *result = execute_minishell_command(command, &ctx);
	cr_assert_not_null(result, "append redirection should work");
	cr_assert(strstr(result->stdout_output, "first") != NULL &&
			  strstr(result->stdout_output, "second") != NULL,
		"append should add to existing content");
	
	unlink(temp_file); /* Clean up */
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(redirections, heredoc_redirection)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing heredoc redirection");
	
	t_test_result *result = execute_minishell_command("cat << EOF\nhello\nworld\nEOF", &ctx);
	cr_assert_not_null(result, "heredoc should work");
	cr_assert(strstr(result->stdout_output, "hello") != NULL &&
			  strstr(result->stdout_output, "world") != NULL,
		"heredoc should provide input to command");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Pipes */
Test(pipes, simple_pipe)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing simple pipe");
	
	t_test_result *result = execute_minishell_command("echo hello | cat", &ctx);
	cr_assert_not_null(result, "simple pipe should work");
	cr_assert(strstr(result->stdout_output, "hello") != NULL,
		"pipe should pass output between commands");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(pipes, multiple_pipes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing multiple pipes");
	
	t_test_result *result = execute_minishell_command("echo hello world | cat | cat", &ctx);
	cr_assert_not_null(result, "multiple pipes should work");
	cr_assert(strstr(result->stdout_output, "hello world") != NULL,
		"multiple pipes should pass data through");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(pipes, pipe_with_failing_command)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing pipe with failing command");
	
	t_test_result *result = execute_minishell_command("ls /nonexistent | grep test | cat", &ctx);
	cr_assert_not_null(result, "pipe with failing command should work");
	/* Should handle error gracefully */
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(pipes, pipes_with_redirections)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing pipes combined with redirections");
	
	char *temp_file = "/tmp/minishell_pipe_redirect";
	char command[256];
	snprintf(command, sizeof(command), "echo hello | cat > %s\ncat %s", temp_file, temp_file);
	
	t_test_result *result = execute_minishell_command(command, &ctx);
	cr_assert_not_null(result, "pipes with redirections should work");
	cr_assert(strstr(result->stdout_output, "hello") != NULL,
		"pipe output should be redirected to file");
	
	unlink(temp_file); /* Clean up */
	free_test_result(result);
	cleanup_test_environment(&ctx);
}