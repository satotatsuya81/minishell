/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_signals_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/18 16:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "evaluation_test_suite.h"
#include <signal.h>
#include <sys/wait.h>

/* Test Suite: Signal Handling */
Test(signals, ctrl_c_empty_prompt, .timeout = 5)
{
	cr_log_info("Testing Ctrl+C behavior on empty prompt");
	
	pid_t pid;
	int status;
	int pipefd[2];
	
	cr_assert_eq(pipe(pipefd), 0, "Pipe creation should succeed");
	
	pid = fork();
	cr_assert_neq(pid, -1, "Fork should succeed");
	
	if (pid == 0)
	{
		/* Child process - run minishell */
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		
		execl("./minishell", "minishell", NULL);
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process - send SIGINT and check behavior */
		close(pipefd[1]);
		
		usleep(100000); /* Wait for minishell to start */
		kill(pid, SIGINT);
		usleep(100000); /* Wait for signal handling */
		kill(pid, SIGTERM); /* Terminate minishell */
		
		waitpid(pid, &status, 0);
		close(pipefd[0]);
		
		/* Ctrl+C should not terminate minishell, SIGTERM should */
		cr_assert(WIFSIGNALED(status), "Minishell should be terminated by SIGTERM");
		cr_assert_eq(WTERMSIG(status), SIGTERM, "Should be terminated by SIGTERM, not SIGINT");
	}
}

Test(signals, ctrl_backslash_empty_prompt, .timeout = 5)
{
	cr_log_info("Testing Ctrl+\\ behavior on empty prompt");
	
	pid_t pid;
	int status;
	
	pid = fork();
	cr_assert_neq(pid, -1, "Fork should succeed");
	
	if (pid == 0)
	{
		/* Child process - run minishell */
		execl("./minishell", "minishell", NULL);
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process - send SIGQUIT */
		usleep(100000); /* Wait for minishell to start */
		kill(pid, SIGQUIT);
		usleep(100000); /* Wait for signal handling */
		kill(pid, SIGTERM); /* Terminate minishell */
		
		waitpid(pid, &status, 0);
		
		/* SIGQUIT should be ignored in interactive mode */
		cr_assert(WIFSIGNALED(status), "Minishell should be terminated by SIGTERM");
		cr_assert_eq(WTERMSIG(status), SIGTERM, "Should be terminated by SIGTERM, not SIGQUIT");
	}
}

Test(signals, ctrl_d_empty_prompt, .timeout = 5)
{
	cr_log_info("Testing Ctrl+D (EOF) behavior on empty prompt");
	
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	/* Send EOF to minishell */
	t_test_result *result = execute_minishell_command("", &ctx);
	cr_assert_not_null(result, "EOF handling should work");
	/* EOF should cause clean exit */
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(signals, signal_during_command_execution, .timeout = 10)
{
	cr_log_info("Testing signal handling during command execution");
	
	pid_t pid;
	int status;
	int pipefd_in[2], pipefd_out[2];
	
	cr_assert_eq(pipe(pipefd_in), 0, "Input pipe creation should succeed");
	cr_assert_eq(pipe(pipefd_out), 0, "Output pipe creation should succeed");
	
	pid = fork();
	cr_assert_neq(pid, -1, "Fork should succeed");
	
	if (pid == 0)
	{
		/* Child process - run minishell */
		close(pipefd_in[1]);
		close(pipefd_out[0]);
		
		dup2(pipefd_in[0], STDIN_FILENO);
		dup2(pipefd_out[1], STDOUT_FILENO);
		
		close(pipefd_in[0]);
		close(pipefd_out[1]);
		
		execl("./minishell", "minishell", NULL);
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process */
		close(pipefd_in[0]);
		close(pipefd_out[1]);
		
		/* Send a blocking command */
		write(pipefd_in[1], "sleep 5\n", 8);
		usleep(500000); /* Wait for command to start */
		
		/* Send SIGINT to the process group */
		kill(pid, SIGINT);
		usleep(100000);
		
		/* Send exit command */
		write(pipefd_in[1], "echo $?\nexit\n", 13);
		close(pipefd_in[1]);
		
		waitpid(pid, &status, 0);
		close(pipefd_out[0]);
		
		/* Should handle signal properly */
		cr_assert(WIFEXITED(status), "Minishell should exit normally after signal");
	}
}

/* Test Suite: Double Quotes */
Test(double_quotes, basic_double_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic double quote functionality");
	
	t_test_result *result = execute_minishell_command("echo \"hello world\"", &ctx);
	cr_assert_not_null(result, "Double quoted string should work");
	cr_assert(strstr(result->stdout_output, "hello world") != NULL,
		"Double quotes should preserve whitespace");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(double_quotes, variable_expansion_in_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing variable expansion in double quotes");
	
	t_test_result *result = execute_minishell_command("echo \"$USER\"", &ctx);
	cr_assert_not_null(result, "Variable expansion in double quotes should work");
	/* Should expand $USER variable */
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(double_quotes, special_characters_in_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing special characters in double quotes");
	
	t_test_result *result = execute_minishell_command("echo \"cat file | grep pattern\"", &ctx);
	cr_assert_not_null(result, "Special characters in quotes should be literal");
	cr_assert(strstr(result->stdout_output, "cat file | grep pattern") != NULL,
		"Pipes and redirections should be literal in quotes");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(double_quotes, empty_double_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing empty double quotes");
	
	t_test_result *result = execute_minishell_command("echo \"\"", &ctx);
	cr_assert_not_null(result, "Empty double quotes should work");
	cr_assert_eq(result->exit_code, 0, "Empty quotes should not cause errors");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Single Quotes */
Test(single_quotes, basic_single_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing basic single quote functionality");
	
	t_test_result *result = execute_minishell_command("echo 'hello world'", &ctx);
	cr_assert_not_null(result, "Single quoted string should work");
	cr_assert(strstr(result->stdout_output, "hello world") != NULL,
		"Single quotes should preserve whitespace");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(single_quotes, no_variable_expansion)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing that variables are NOT expanded in single quotes");
	
	t_test_result *result = execute_minishell_command("echo '$USER'", &ctx);
	cr_assert_not_null(result, "Variable in single quotes should work");
	cr_assert(strstr(result->stdout_output, "$USER") != NULL,
		"$USER should be literal, not expanded");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(single_quotes, special_characters_literal)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing special characters are literal in single quotes");
	
	t_test_result *result = execute_minishell_command("echo 'cat file | grep pattern > output'", &ctx);
	cr_assert_not_null(result, "Special characters should be literal");
	cr_assert(strstr(result->stdout_output, "cat file | grep pattern > output") != NULL,
		"All special characters should be literal in single quotes");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(single_quotes, empty_single_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing empty single quotes");
	
	t_test_result *result = execute_minishell_command("echo ''", &ctx);
	cr_assert_not_null(result, "Empty single quotes should work");
	cr_assert_eq(result->exit_code, 0, "Empty quotes should not cause errors");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(single_quotes, dollar_question_literal)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing $? is literal in single quotes");
	
	t_test_result *result = execute_minishell_command("echo '$?'", &ctx);
	cr_assert_not_null(result, "$? in single quotes should work");
	cr_assert(strstr(result->stdout_output, "$?") != NULL,
		"$? should be literal, not expanded");
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

/* Test Suite: Quote Error Handling */
Test(quotes, unclosed_double_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing unclosed double quotes error handling");
	
	t_test_result *result = execute_minishell_command("echo \"unclosed", &ctx);
	cr_assert_not_null(result, "Unclosed quotes should be handled");
	/* Should either prompt for more input or show error */
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}

Test(quotes, unclosed_single_quotes)
{
	t_test_context ctx;
	setup_test_environment(&ctx);
	
	cr_log_info("Testing unclosed single quotes error handling");
	
	t_test_result *result = execute_minishell_command("echo 'unclosed", &ctx);
	cr_assert_not_null(result, "Unclosed quotes should be handled");
	/* Should either prompt for more input or show error */
	
	free_test_result(result);
	cleanup_test_environment(&ctx);
}