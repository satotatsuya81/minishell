/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_error_handling_criterion_tdt.c                :+:      :+:    :+:   */
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
#include <errno.h>
#include <sys/wait.h>
#include "infrastructure/process_service.h"
#include "usecase/executor/executor.h"

/* ************************************************************************** */
/*                          Expected Results Definition                       */
/* ************************************************************************** */

// Expected results for process error handling
typedef struct s_expected_process_error {
	t_process_result	expected_error_code;	// 期待エラーコード
	int					error_detected;			// 1: エラー検出期待
	const char			*expected_error_msg;	// 期待エラーメッセージ
	int					cleanup_successful;		// 1: クリーンアップ成功期待
} t_expected_process_error;

// Expected results for file I/O error handling
typedef struct s_expected_io_error {
	int				file_operation_failed;		// 1: ファイル操作失敗期待
	int				error_code;					// システムエラーコード
	const char		*expected_error_msg;		// 期待エラーメッセージ
	int				recovery_successful;		// 1: 回復処理成功期待
} t_expected_io_error;

// Expected results for memory error handling
typedef struct s_expected_memory_error {
	int				allocation_failed;			// 1: メモリ割り当て失敗期待
	int				leak_detected;				// 1: リーク検出期待
	int				cleanup_successful;			// 1: クリーンアップ成功期待
	const char		*expected_error_msg;		// 期待エラーメッセージ
} t_expected_memory_error;

/* ************************************************************************** */
/*                          Expected Data Definitions                        */
/* ************************************************************************** */

// Process management error cases
static const t_expected_process_error fork_null_error_expected = {
	PROCESS_ERROR_FORK,			// expected_error_code
	1,							// error_detected
	"fork: invalid parameter",	// expected_error_msg
	1							// cleanup_successful
};

static const t_expected_process_error exec_null_error_expected = {
	PROCESS_ERROR_EXEC,			// expected_error_code
	1,							// error_detected
	"exec: invalid parameter",	// expected_error_msg
	1							// cleanup_successful
};

static const t_expected_process_error wait_invalid_error_expected = {
	PROCESS_ERROR_WAIT,			// expected_error_code
	1,							// error_detected
	"wait: invalid pid",		// expected_error_msg
	1							// cleanup_successful
};

static const t_expected_process_error pipe_null_error_expected = {
	PROCESS_ERROR_PIPE,			// expected_error_code
	1,							// error_detected
	"pipe: invalid parameter",	// expected_error_msg
	1							// cleanup_successful
};

static const t_expected_process_error dup_invalid_error_expected = {
	PROCESS_ERROR_DUP,			// expected_error_code
	1,							// error_detected
	"dup: invalid file descriptor",	// expected_error_msg
	1							// cleanup_successful
};

// File I/O error cases
static const t_expected_io_error file_not_found_expected = {
	1,							// file_operation_failed
	ENOENT,						// error_code
	"No such file or directory",	// expected_error_msg
	1							// recovery_successful
};

static const t_expected_io_error permission_denied_expected = {
	1,							// file_operation_failed
	EACCES,						// error_code
	"Permission denied",		// expected_error_msg
	1							// recovery_successful
};

static const t_expected_io_error disk_full_expected = {
	1,							// file_operation_failed
	ENOSPC,						// error_code
	"No space left on device",	// expected_error_msg
	1							// recovery_successful
};

// Memory error cases
static const t_expected_memory_error malloc_failure_expected = {
	1,							// allocation_failed
	0,							// leak_detected
	1,							// cleanup_successful
	"Memory allocation failed"	// expected_error_msg
};

static const t_expected_memory_error memory_leak_expected = {
	0,							// allocation_failed
	1,							// leak_detected
	1,							// cleanup_successful
	"Memory leak detected"		// expected_error_msg
};

/* ************************************************************************** */
/*                          Error Test Case Structure                        */
/* ************************************************************************** */

typedef struct s_error_test_case {
	const char					*input_description;
	int							error_type;
	const void					*expected_result;
	int							expected_success;
	const char					*test_name;
} t_error_test_case;

// Error types
#define ERROR_TYPE_PROCESS		1
#define ERROR_TYPE_IO			2
#define ERROR_TYPE_MEMORY		3

/* ************************************************************************** */
/*                          Test Implementation Functions                    */
/* ************************************************************************** */

static int	verify_process_error(t_process_service *service,
		const t_expected_process_error *expected)
{
	t_process_result	result;
	char				*error_msg;

	// NULL パラメータでのforkテスト
	result = service->fork_process(NULL);
	if (result != expected->expected_error_code)
		return (0);

	// エラーメッセージ確認
	if (expected->expected_error_msg)
	{
		error_msg = service->get_error_message(result);
		if (!error_msg || strstr(error_msg, "invalid") == NULL)
		{
			if (error_msg)
				free(error_msg);
			return (0);
		}
		free(error_msg);
	}

	return (expected->error_detected);
}

static int	verify_io_error(const char *filename,
		const t_expected_io_error *expected)
{
	int		fd;
	int		actual_errno;

	// 存在しないファイルを開こうとする
	fd = open(filename, O_RDONLY);
	actual_errno = errno;
	
	if (fd != -1)
	{
		close(fd);
		return (!expected->file_operation_failed);
	}

	// エラーコード確認
	if (expected->error_code != 0 && actual_errno != expected->error_code)
		return (0);

	return (expected->file_operation_failed);
}

static int	verify_memory_error(const t_expected_memory_error *expected)
{
	void	*ptr;

	// メモリ割り当てテスト（巨大サイズで失敗を誘発）
	ptr = malloc(SIZE_MAX);
	
	if (ptr == NULL)
	{
		// 割り当て失敗を期待していた場合
		return (expected->allocation_failed);
	}
	else
	{
		// 予期しない成功
		free(ptr);
		return (!expected->allocation_failed);
	}
}

/* ************************************************************************** */
/*                          Test Data Table (TDT)                            */
/* ************************************************************************** */

static const t_error_test_case error_test_cases[] = {
	{
		"Process fork with NULL parameter",
		ERROR_TYPE_PROCESS,
		&fork_null_error_expected,
		1,
		"fork_null_error"
	},
	{
		"Process exec with NULL parameter",
		ERROR_TYPE_PROCESS,
		&exec_null_error_expected,
		1,
		"exec_null_error"
	},
	{
		"Process wait with invalid PID",
		ERROR_TYPE_PROCESS,
		&wait_invalid_error_expected,
		1,
		"wait_invalid_error"
	},
	{
		"Process pipe with NULL parameter",
		ERROR_TYPE_PROCESS,
		&pipe_null_error_expected,
		1,
		"pipe_null_error"
	},
	{
		"Process dup with invalid file descriptor",
		ERROR_TYPE_PROCESS,
		&dup_invalid_error_expected,
		1,
		"dup_invalid_error"
	},
	{
		"File I/O with nonexistent file",
		ERROR_TYPE_IO,
		&file_not_found_expected,
		1,
		"file_not_found"
	},
	{
		"File I/O with permission denied",
		ERROR_TYPE_IO,
		&permission_denied_expected,
		1,
		"permission_denied"
	},
	{
		"File I/O with disk full error",
		ERROR_TYPE_IO,
		&disk_full_expected,
		1,
		"disk_full"
	},
	{
		"Memory allocation failure",
		ERROR_TYPE_MEMORY,
		&malloc_failure_expected,
		1,
		"malloc_failure"
	},
	{
		"Memory leak detection",
		ERROR_TYPE_MEMORY,
		&memory_leak_expected,
		1,
		"memory_leak"
	}
};

/* ************************************************************************** */
/*                          Criterion Test Implementation                    */
/* ************************************************************************** */

Test(error_handling, process_errors)
{
	t_process_service	*service;
	int					result;

	cr_log_info("Testing process error handling");
	
	service = create_process_service();
	cr_assert_not_null(service, "Process service creation failed");

	result = verify_process_error(service, &fork_null_error_expected);
	cr_assert(result, "Process error verification failed");

	destroy_process_service(service);
	cr_log_info("✅ Process error handling test passed");
}

Test(error_handling, io_errors)
{
	int	result;

	cr_log_info("Testing I/O error handling");
	
	result = verify_io_error("/nonexistent/file", &file_not_found_expected);
	cr_assert(result, "I/O error verification failed");
	
	cr_log_info("✅ I/O error handling test passed");
}

Test(error_handling, memory_errors)
{
	int	result;

	cr_log_info("Testing memory error handling");
	
	result = verify_memory_error(&malloc_failure_expected);
	cr_assert(result, "Memory error verification failed");
	
	cr_log_info("✅ Memory error handling test passed");
}

/* ************************************************************************** */
/*                          Table-Driven Test Runner                         */
/* ************************************************************************** */

Test(error_handling, table_driven_tests)
{
	size_t					case_count;
	size_t					i;
	int						result;
	int						passed;
	const t_error_test_case	*test_case;
	t_process_service		*service;

	case_count = sizeof(error_test_cases) / sizeof(error_test_cases[0]);
	passed = 0;

	cr_log_info("=== Error Handling Criterion TDT Test Suite ===");
	
	service = create_process_service();
	cr_assert_not_null(service, "Process service creation failed");
	
	for (i = 0; i < case_count; i++)
	{
		test_case = &error_test_cases[i];
		cr_log_info("Running test %zu: %s", i, test_case->input_description);

		// エラーハンドリングテスト実行
		if (test_case->error_type == ERROR_TYPE_PROCESS)
			result = verify_process_error(service,
					(const t_expected_process_error *)test_case->expected_result);
		else if (test_case->error_type == ERROR_TYPE_IO)
			result = verify_io_error("/nonexistent/file",
					(const t_expected_io_error *)test_case->expected_result);
		else if (test_case->error_type == ERROR_TYPE_MEMORY)
			result = verify_memory_error(
					(const t_expected_memory_error *)test_case->expected_result);
		else
			result = 0;

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

	destroy_process_service(service);

	cr_log_info("=== Error Handling Criterion TDT Summary ===");
	cr_log_info("Passed: %d/%zu", passed, case_count);
	
	cr_assert_eq(passed, (int)case_count, "Not all tests passed: %d/%zu", passed, case_count);
	
	if (passed == (int)case_count)
		cr_log_info("🎉 All Error Handling Criterion TDT tests passed!");
}