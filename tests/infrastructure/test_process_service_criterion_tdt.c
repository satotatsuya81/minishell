/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_process_service_criterion_tdt.c              :+:      :+:    :+:   */
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
#include <unistd.h>
#include <sys/wait.h>
#include "infrastructure/process_service.h"

/* ************************************************************************** */
/*                          Expected Results Definition                       */
/* ************************************************************************** */

// Expected results for process service creation
typedef struct s_expected_service {
	int				service_created;		// 1: サービス作成成功期待
	int				all_functions_present;	// 1: 全関数ポインタ存在期待
	int				cleanup_successful;		// 1: クリーンアップ成功期待
} t_expected_service;

// Expected results for pipe operations
typedef struct s_expected_pipe {
	int				pipe_created;			// 1: パイプ作成成功期待
	int				read_fd_valid;			// 1: 読み込みFD有効期待
	int				write_fd_valid;			// 1: 書き込みFD有効期待
	int				pipe_closed;			// 1: パイプクローズ成功期待
} t_expected_pipe;

// Expected results for fork/wait operations
typedef struct s_expected_fork {
	int				fork_successful;		// 1: fork成功期待
	int				child_exit_code;		// 期待子プロセス終了コード
	int				wait_successful;		// 1: wait成功期待
} t_expected_fork;

// Expected results for error conditions
typedef struct s_expected_error {
	t_process_result	fork_null_result;		// fork(NULL)期待結果
	t_process_result	exec_null_result;		// exec(NULL)期待結果
	t_process_result	wait_invalid_result;	// wait(-1)期待結果
	t_process_result	pipe_null_result;		// pipe(NULL)期待結果
	t_process_result	dup_invalid_result;		// dup(-1)期待結果
} t_expected_error;

/* ************************************************************************** */
/*                          Expected Data Definitions                        */
/* ************************************************************************** */

// Service creation expected result
static const t_expected_service service_creation_expected = {
	1,	// service_created
	1,	// all_functions_present
	1	// cleanup_successful
};

// Pipe operations expected result
static const t_expected_pipe pipe_operations_expected = {
	1,	// pipe_created
	1,	// read_fd_valid
	1,	// write_fd_valid
	1	// pipe_closed
};

// Fork/wait expected result (exit code 42)
static const t_expected_fork fork_wait_expected = {
	1,	// fork_successful
	42,	// child_exit_code
	1	// wait_successful
};

// Error conditions expected results
static const t_expected_error error_conditions_expected = {
	PROCESS_ERROR_FORK,	// fork_null_result
	PROCESS_ERROR_EXEC,	// exec_null_result
	PROCESS_ERROR_WAIT,	// wait_invalid_result
	PROCESS_ERROR_PIPE,	// pipe_null_result
	PROCESS_ERROR_DUP	// dup_invalid_result
};

/* ************************************************************************** */
/*                          Process Service Test Case Structure              */
/* ************************************************************************** */

typedef struct s_process_service_test_case {
	const char					*input_description;
	int							expected_success;
	const void					*expected_result;
	int							test_type;
	const char					*test_name;
} t_process_service_test_case;

// Test types
#define TEST_SERVICE_CREATION	1
#define TEST_PIPE_OPERATIONS	2
#define TEST_FORK_WAIT			3
#define TEST_ERROR_CONDITIONS	4

/* ************************************************************************** */
/*                          Test Implementation Functions                    */
/* ************************************************************************** */

static int	verify_service_creation(t_process_service *service,
		const t_expected_service *expected)
{
	if (!service && expected->service_created)
		return (0);
	if (!expected->all_functions_present)
		return (1);
	if (!service->fork_process || !service->exec_command ||
		!service->wait_process || !service->wait_all_processes ||
		!service->create_pipe || !service->close_pipe ||
		!service->duplicate_fd || !service->close_fd ||
		!service->is_process_running || !service->get_error_message)
		return (0);
	return (1);
}

static int	verify_pipe_operations(t_process_service *service,
		const t_expected_pipe *expected)
{
	t_pipe_info			pipe_info;
	t_process_result	result;

	result = service->create_pipe(&pipe_info);
	if ((result == PROCESS_SUCCESS) != expected->pipe_created)
		return (0);
	if (expected->read_fd_valid && pipe_info.read_fd < 0)
		return (0);
	if (expected->write_fd_valid && pipe_info.write_fd < 0)
		return (0);
	if (expected->pipe_closed)
	{
		result = service->close_pipe(&pipe_info);
		if (result != PROCESS_SUCCESS)
			return (0);
	}
	return (1);
}

static int	verify_fork_wait(t_process_service *service,
		const t_expected_fork *expected)
{
	pid_t				pid;
	t_process_result	result;
	int					exit_status;

	result = service->fork_process(&pid);
	if ((result == PROCESS_SUCCESS) != expected->fork_successful)
		return (0);
	if (pid == 0)
		exit(expected->child_exit_code);
	result = service->wait_process(pid, &exit_status);
	if ((result == PROCESS_SUCCESS) != expected->wait_successful)
		return (0);
	if (exit_status != expected->child_exit_code)
		return (0);
	return (1);
}

static int	verify_error_conditions(t_process_service *service,
		const t_expected_error *expected)
{
	if (service->fork_process(NULL) != expected->fork_null_result)
		return (0);
	if (service->exec_command(NULL, NULL, NULL) != expected->exec_null_result)
		return (0);
	if (service->wait_process(-1, NULL) != expected->wait_invalid_result)
		return (0);
	if (service->create_pipe(NULL) != expected->pipe_null_result)
		return (0);
	if (service->duplicate_fd(-1, 0) != expected->dup_invalid_result)
		return (0);
	return (1);
}

/* ************************************************************************** */
/*                          Test Data Table (TDT)                            */
/* ************************************************************************** */

static const t_process_service_test_case process_service_test_cases[] = {
	{
		"Process service creation with all function pointers",
		1,
		&service_creation_expected,
		TEST_SERVICE_CREATION,
		"service_creation"
	},
	{
		"Pipe creation and cleanup operations",
		1,
		&pipe_operations_expected,
		TEST_PIPE_OPERATIONS,
		"pipe_operations"
	},
	{
		"Fork process and wait with exit code 42",
		1,
		&fork_wait_expected,
		TEST_FORK_WAIT,
		"fork_wait"
	},
	{
		"Error conditions with NULL parameters and invalid values",
		1,
		&error_conditions_expected,
		TEST_ERROR_CONDITIONS,
		"error_conditions"
	}
};

/* ************************************************************************** */
/*                          Criterion Test Implementation                    */
/* ************************************************************************** */

Test(infrastructure_process_service, service_creation)
{
	t_process_service	*service;
	int					result;

	cr_log_info("Testing process service creation");
	
	service = create_process_service();
	cr_assert_not_null(service, "Process service creation failed");

	result = verify_service_creation(service, &service_creation_expected);
	cr_assert(result, "Service creation verification failed");

	if (service)
		destroy_process_service(service);
		
	cr_log_info("✅ Service creation test passed");
}

Test(infrastructure_process_service, pipe_operations)
{
	t_process_service	*service;
	int					result;

	cr_log_info("Testing pipe operations");
	
	service = create_process_service();
	cr_assert_not_null(service, "Process service creation failed");

	result = verify_pipe_operations(service, &pipe_operations_expected);
	cr_assert(result, "Pipe operations verification failed");

	destroy_process_service(service);
	cr_log_info("✅ Pipe operations test passed");
}

Test(infrastructure_process_service, fork_wait)
{
	t_process_service	*service;
	int					result;

	cr_log_info("Testing fork and wait operations");
	
	service = create_process_service();
	cr_assert_not_null(service, "Process service creation failed");

	result = verify_fork_wait(service, &fork_wait_expected);
	cr_assert(result, "Fork/wait operations verification failed");

	destroy_process_service(service);
	cr_log_info("✅ Fork/wait operations test passed");
}

Test(infrastructure_process_service, error_conditions)
{
	t_process_service	*service;
	int					result;

	cr_log_info("Testing error condition handling");
	
	service = create_process_service();
	cr_assert_not_null(service, "Process service creation failed");

	result = verify_error_conditions(service, &error_conditions_expected);
	cr_assert(result, "Error conditions verification failed");

	destroy_process_service(service);
	cr_log_info("✅ Error conditions test passed");
}

/* ************************************************************************** */
/*                          Table-Driven Test Runner                         */
/* ************************************************************************** */

Test(infrastructure_process_service, table_driven_tests)
{
	size_t						case_count;
	size_t						i;
	t_process_service			*service;
	int							result;
	int							passed;
	const t_process_service_test_case	*test_case;

	case_count = sizeof(process_service_test_cases) / sizeof(process_service_test_cases[0]);
	passed = 0;

	cr_log_info("=== Process Service Criterion TDT Test Suite ===");
	
	for (i = 0; i < case_count; i++)
	{
		test_case = &process_service_test_cases[i];
		cr_log_info("Running test %zu: %s", i, test_case->input_description);

		service = create_process_service();
		if (!service && test_case->expected_success)
		{
			cr_log_error("❌ Test %zu failed: Service creation failed", i);
			continue;
		}

		if (test_case->test_type == TEST_SERVICE_CREATION)
			result = verify_service_creation(service,
					(const t_expected_service *)test_case->expected_result);
		else if (test_case->test_type == TEST_PIPE_OPERATIONS)
			result = verify_pipe_operations(service,
					(const t_expected_pipe *)test_case->expected_result);
		else if (test_case->test_type == TEST_FORK_WAIT)
			result = verify_fork_wait(service,
					(const t_expected_fork *)test_case->expected_result);
		else if (test_case->test_type == TEST_ERROR_CONDITIONS)
			result = verify_error_conditions(service,
					(const t_expected_error *)test_case->expected_result);
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

		if (service)
			destroy_process_service(service);
	}

	cr_log_info("=== Process Service Criterion TDT Summary ===");
	cr_log_info("Passed: %d/%zu", passed, case_count);
	
	cr_assert_eq(passed, (int)case_count, "Not all tests passed: %d/%zu", passed, case_count);
	
	if (passed == (int)case_count)
		cr_log_info("🎉 All Process Service Criterion TDT tests passed!");
}