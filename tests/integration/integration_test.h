/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integration_test.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claude <claude@anthropic.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato            #+#    #+#             */
/*   Updated: 2025/01/06 00:00:00 by tatsato           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTEGRATION_TEST_H
# define INTEGRATION_TEST_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include "usecase/lexer/lexer.h"
# include "adapters/parser/parser.h"
# include "domain/token.h"

/* Test case structure for integration tests */
typedef struct {
    const char *input;
    const char *description;
    int should_succeed;
    int expected_pipeline_count;
    int expected_cmd_count_in_first_pipeline;
    const char *expected_first_command;
    const char *expected_error_pattern;
} integration_test_case;

/* Test results tracking */
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
} test_results;

/* Function prototypes */
int count_pipelines(t_pipeline *ast);
int count_commands_in_pipeline(t_pipeline *pipeline);
void run_integration_test(const integration_test_case *test_case, test_results *results);

#endif /* INTEGRATION_TEST_H */
