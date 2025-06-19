/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_comprehensive_criterion_tdt.c                 :+:      :+:    :+:   */
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
#include <sys/wait.h>
#include "usecase/lexer/lexer.h"
#include "adapters/parser/parser.h"
#include "usecase/executor/executor.h"

/* ************************************************************************** */
/*                          Expected Results Definition                       */
/* ************************************************************************** */

// Expected results for comprehensive minishell operations
typedef struct s_expected_integration {
	int				lexer_success;			// 1: 字句解析成功期待
	int				parser_success;			// 1: 構文解析成功期待
	int				executor_success;		// 1: 実行成功期待
	int				expected_token_count;	// 期待トークン数
	int				expected_exit_code;		// 期待終了コード
	const char		*expected_output;		// 期待出力内容
} t_expected_integration;

/* ************************************************************************** */
/*                          Expected Data Definitions                        */
/* ************************************************************************** */

// Simple builtin command integration
static const t_expected_integration simple_builtin_expected = {
	1,			// lexer_success
	1,			// parser_success
	1,			// executor_success
	2,			// expected_token_count (echo, hello)
	0,			// expected_exit_code
	"hello"		// expected_output
};

// Pipe command integration
static const t_expected_integration pipe_command_expected = {
	1,			// lexer_success
	1,			// parser_success
	1,			// executor_success
	4,			// expected_token_count (ls, |, grep, main)
	0,			// expected_exit_code
	NULL		// expected_output (variable)
};

// Redirection integration
static const t_expected_integration redirection_expected = {
	1,			// lexer_success
	1,			// parser_success
	1,			// executor_success
	4,			// expected_token_count (echo, hello, >, file)
	0,			// expected_exit_code
	NULL		// expected_output (to file)
};

// Complex command integration
static const t_expected_integration complex_command_expected = {
	1,			// lexer_success
	1,			// parser_success
	1,			// executor_success
	7,			// expected_token_count (echo, test, |, grep, t, >, out)
	0,			// expected_exit_code
	NULL		// expected_output (to file)
};

// Error case integration
static const t_expected_integration error_case_expected = {
	1,			// lexer_success
	0,			// parser_success (構文エラー期待)
	0,			// executor_success
	0,			// expected_token_count
	1,			// expected_exit_code (エラー)
	NULL		// expected_output
};

/* ************************************************************************** */
/*                          Integration Test Case Structure                  */
/* ************************************************************************** */

typedef struct s_integration_test_case {
	const char					*input_description;
	const char					*command_line;
	int							expected_success;
	const t_expected_integration	*expected_result;
	const char					*test_name;
} t_integration_test_case;

/* ************************************************************************** */
/*                          Test Implementation Functions                    */
/* ************************************************************************** */

static int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static int	verify_integration_pipeline(const char *command_line,
		const t_expected_integration *expected)
{
	t_token			*tokens;
	t_pipeline		*ast;
	int				token_count;
	int				lexer_result;
	int				parser_result;

	// 字句解析テスト
	tokens = lexer_tokenize(command_line);
	lexer_result = (tokens != NULL);
	
	if (lexer_result != expected->lexer_success)
	{
		if (tokens)
			lexer_free_tokens(tokens);
		return (0);
	}

	if (!expected->lexer_success)
	{
		if (tokens)
			lexer_free_tokens(tokens);
		return (1);  // エラーケース成功
	}

	// トークン数確認
	token_count = count_tokens(tokens);
	if (expected->expected_token_count > 0 && 
		token_count != expected->expected_token_count)
	{
		lexer_free_tokens(tokens);
		return (0);
	}

	// 構文解析テスト
	ast = parse_tokens(tokens);
	parser_result = (ast != NULL);
	
	if (parser_result != expected->parser_success)
	{
		lexer_free_tokens(tokens);
		if (ast)
			free_pipeline(ast);
		return (0);
	}

	// クリーンアップ
	lexer_free_tokens(tokens);
	if (ast)
		free_pipeline(ast);

	return (1);
}

/* ************************************************************************** */
/*                          Test Data Table (TDT)                            */
/* ************************************************************************** */

static const t_integration_test_case integration_test_cases[] = {
	{
		"Simple builtin command: echo hello",
		"echo hello",
		1,
		&simple_builtin_expected,
		"simple_builtin"
	},
	{
		"Pipe command: ls | grep main",
		"ls | grep main",
		1,
		&pipe_command_expected,
		"pipe_command"
	},
	{
		"Output redirection: echo hello > file.txt",
		"echo hello > file.txt",
		1,
		&redirection_expected,
		"output_redirection"
	},
	{
		"Complex command: echo test | grep t > out.txt",
		"echo test | grep t > out.txt",
		1,
		&complex_command_expected,
		"complex_command"
	},
	{
		"Syntax error case: invalid command structure",
		"echo | |",
		0,
		&error_case_expected,
		"syntax_error"
	}
};

/* ************************************************************************** */
/*                          Criterion Test Implementation                    */
/* ************************************************************************** */

Test(integration_comprehensive, simple_builtin)
{
	int	result;

	cr_log_info("Testing simple builtin integration");
	
	result = verify_integration_pipeline("echo hello", &simple_builtin_expected);
	cr_assert(result, "Simple builtin integration verification failed");
	
	cr_log_info("✅ Simple builtin integration test passed");
}

Test(integration_comprehensive, pipe_command)
{
	int	result;

	cr_log_info("Testing pipe command integration");
	
	result = verify_integration_pipeline("ls | grep main", &pipe_command_expected);
	cr_assert(result, "Pipe command integration verification failed");
	
	cr_log_info("✅ Pipe command integration test passed");
}

Test(integration_comprehensive, output_redirection)
{
	int	result;

	cr_log_info("Testing output redirection integration");
	
	result = verify_integration_pipeline("echo hello > file.txt", &redirection_expected);
	cr_assert(result, "Output redirection integration verification failed");
	
	cr_log_info("✅ Output redirection integration test passed");
}

Test(integration_comprehensive, complex_command)
{
	int	result;

	cr_log_info("Testing complex command integration");
	
	result = verify_integration_pipeline("echo test | grep t > out.txt", 
			&complex_command_expected);
	cr_assert(result, "Complex command integration verification failed");
	
	cr_log_info("✅ Complex command integration test passed");
}

Test(integration_comprehensive, syntax_error)
{
	int	result;

	cr_log_info("Testing syntax error handling");
	
	result = verify_integration_pipeline("echo | |", &error_case_expected);
	cr_assert(result, "Syntax error integration verification failed");
	
	cr_log_info("✅ Syntax error integration test passed");
}

/* ************************************************************************** */
/*                          Table-Driven Test Runner                         */
/* ************************************************************************** */

Test(integration_comprehensive, table_driven_tests)
{
	size_t						case_count;
	size_t						i;
	int							result;
	int							passed;
	const t_integration_test_case	*test_case;

	case_count = sizeof(integration_test_cases) / sizeof(integration_test_cases[0]);
	passed = 0;

	cr_log_info("=== Comprehensive Integration Criterion TDT Test Suite ===");
	
	for (i = 0; i < case_count; i++)
	{
		test_case = &integration_test_cases[i];
		cr_log_info("Running test %zu: %s", i, test_case->input_description);

		// 統合テスト実行・検証
		result = verify_integration_pipeline(test_case->command_line,
				test_case->expected_result);

		cr_assert_eq(result, test_case->expected_success,
				"Test '%s' failed: expected %d, got %d",
				test_case->test_name, test_case->expected_success, result);

		if (result == test_case->expected_success)
		{
			cr_log_info("✅ Test %zu passed: %s", i, test_case->test_name);
			passed++;
		}
		else
		{
			cr_log_error("❌ Test %zu failed: %s", i, test_case->test_name);
		}
	}

	cr_log_info("=== Comprehensive Integration Criterion TDT Summary ===");
	cr_log_info("Passed: %d/%zu", passed, case_count);
	
	cr_assert_eq(passed, (int)case_count, "Not all tests passed: %d/%zu", passed, case_count);
	
	if (passed == (int)case_count)
		cr_log_info("🎉 All Comprehensive Integration Criterion TDT tests passed!");
}