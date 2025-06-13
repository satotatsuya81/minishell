/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_integration_test.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/14 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include "domain/token.h"
#include "usecase/lexer/lexer.h"
#include "usecase/parser/parser_interface.h"
#include "usecase/executor/executor.h"
#include "domain/env_initializer.h"

typedef struct s_integration_test {
	const char	*input;
	const char	*description;
	int			should_succeed;
	int			expected_exit_code;
	const char	*expected_output_file;
	const char	*expected_file_content;
} t_integration_test;

/* Test utilities */
void	cleanup_test_files(void)
{
	unlink("integration_output.txt");
	unlink("integration_input.txt");
	unlink("integration_append.txt");
	unlink("temp_test.txt");
}

int	file_contains(const char *filename, const char *content)
{
	FILE	*file;
	char	buffer[1024];
	int		found;

	file = fopen(filename, "r");
	if (!file)
		return (0);
	found = 0;
	while (fgets(buffer, sizeof(buffer), file))
	{
		if (strstr(buffer, content))
		{
			found = 1;
			break;
		}
	}
	fclose(file);
	return (found);
}

int	run_integration_test(t_integration_test *test)
{
	t_token_stream	*tokens;
	t_parse_result	*result;
	t_exec_context	*ctx;
	t_env_var		*env;
	int				status;

	printf("Testing: %s -> ", test->description);
	
	/* Step 1: Lexical analysis */
	tokens = lexer(test->input);
	if (!tokens)
	{
		printf("FAILED - Lexer returned NULL\n");
		return (0);
	}
	
	if (tokens->error_message)
	{
		if (test->should_succeed)
		{
			printf("FAILED - Unexpected lexer error: %s\n", tokens->error_message);
			free_tokens(tokens);
			return (0);
		}
		else
		{
			printf("PASSED - Expected lexer error: %s\n", tokens->error_message);
			free_tokens(tokens);
			return (1);
		}
	}
	
	/* Step 2: Parsing */
	result = parse(tokens);
	if (!result)
	{
		printf("FAILED - Parser returned NULL\n");
		free_tokens(tokens);
		return (0);
	}
	
	if (result->error_msg)
	{
		if (test->should_succeed)
		{
			printf("FAILED - Unexpected parser error: %s\n", result->error_msg);
			free_parse_result(result);
			free_tokens(tokens);
			return (0);
		}
		else
		{
			printf("PASSED - Expected parser error: %s\n", result->error_msg);
			free_parse_result(result);
			free_tokens(tokens);
			return (1);
		}
	}
	
	/* Step 3: Execution */
	if (!test->should_succeed)
	{
		printf("FAILED - Expected error but parsing succeeded\n");
		free_parse_result(result);
		free_tokens(tokens);
		return (0);
	}
	
	if (!result->ast)
	{
		printf("FAILED - AST is NULL\n");
		free_parse_result(result);
		free_tokens(tokens);
		return (0);
	}
	
	/* Create execution context */
	env = env_create_from_envp(__environ);
	ctx = create_exec_context(&env);
	if (!ctx)
	{
		printf("FAILED - Failed to create execution context\n");
		free_parse_result(result);
		free_tokens(tokens);
		return (0);
	}
	
	/* Execute */
	status = execute_pipeline_list(result->ast, ctx);
	
	/* Verify results */
	if (test->expected_exit_code >= 0 && status != test->expected_exit_code)
	{
		printf("FAILED - Expected exit code %d, got %d\n", 
			   test->expected_exit_code, status);
		free_exec_context(ctx);
		free_parse_result(result);
		free_tokens(tokens);
		return (0);
	}
	
	if (test->expected_output_file && test->expected_file_content)
	{
		if (!file_contains(test->expected_output_file, test->expected_file_content))
		{
			printf("FAILED - Expected file content not found\n");
			free_exec_context(ctx);
			free_parse_result(result);
			free_tokens(tokens);
			return (0);
		}
	}
	
	printf("PASSED\n");
	
	free_exec_context(ctx);
	free_parse_result(result);
	free_tokens(tokens);
	return (1);
}

int	main(void)
{
	printf("=== Full Integration Test (Lexer + Parser + Executor) ===\n\n");
	
	/* Create test input file */
	FILE *input_file = fopen("integration_input.txt", "w");
	fprintf(input_file, "integration test content\n");
	fclose(input_file);
	
	t_integration_test tests[] = {
		/* Basic command execution */
		{"echo hello", "Basic echo command", 1, 0, NULL, NULL},
		{"pwd", "PWD builtin", 1, 0, NULL, NULL},
		{"env | head -5", "Environment listing", 1, 0, NULL, NULL},
		
		/* Redirection tests */
		{"echo integration > integration_output.txt", "Output redirection", 1, 0, 
		 "integration_output.txt", "integration"},
		{"cat integration_input.txt", "Input file reading", 1, 0, NULL, NULL},
		{"echo append_test >> integration_append.txt", "Append redirection", 1, 0,
		 "integration_append.txt", "append_test"},
		
		/* External commands */
		{"ls .", "External ls command", 1, 0, NULL, NULL},
		{"cat integration_input.txt", "External cat command", 1, 0, NULL, NULL},
		
		/* Builtin commands */
		{"cd /tmp", "Change directory", 1, 0, NULL, NULL},
		{"export TEST_VAR=integration_value", "Export variable", 1, 0, NULL, NULL},
		
		/* Complex commands */
		{"echo 'quoted string'", "Single quoted string", 1, 0, NULL, NULL},
		{"echo \"double quoted\"", "Double quoted string", 1, 0, NULL, NULL},
		
		/* Error cases */
		{"nonexistent_command_12345", "Invalid command", 1, 127, NULL, NULL},
		
		/* Pipe tests (basic) */
		{"echo test", "Simple command for pipe prep", 1, 0, NULL, NULL},
	};
	
	int total_tests = sizeof(tests) / sizeof(tests[0]);
	int passed = 0;
	
	printf("Running %d integration tests...\n\n", total_tests);
	
	for (int i = 0; i < total_tests; i++)
	{
		if (run_integration_test(&tests[i]))
			passed++;
	}
	
	printf("\n=== Integration Test Results ===\n");
	printf("Total tests: %d\n", total_tests);
	printf("Passed: %d\n", passed);
	printf("Failed: %d\n", total_tests - passed);
	printf("Success rate: %.1f%%\n", (float)passed / total_tests * 100);
	
	if (passed == total_tests)
		printf("\n🎉 All integration tests passed!\n");
	else
		printf("\n⚠️  Some integration tests failed.\n");
	
	cleanup_test_files();
	return (passed == total_tests ? 0 : 1);
}