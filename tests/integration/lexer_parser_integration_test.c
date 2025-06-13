#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "usecase/lexer/lexer.h"
#include "usecase/parser/parser.h"
#include "domain/token.h"

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

/* Helper function to count pipelines in AST */
int count_pipelines(t_pipeline *ast)
{
    int count = 0;
    t_pipeline *current = ast;
    
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

/* Helper function to count commands in a pipeline */
int count_commands_in_pipeline(t_pipeline *pipeline)
{
    int count = 0;
    t_cmd *current = pipeline->cmds;
    
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

/* Run a single integration test */
void run_integration_test(const integration_test_case *test_case, test_results *results)
{
    printf("Test: %s... ", test_case->description);
    results->total_tests++;
    
    /* Step 1: Lexer */
    t_token_stream *tokens = lexer(test_case->input);
    if (!tokens) {
        printf("FAILED - Lexer returned NULL\n");
        results->failed_tests++;
        return;
    }
    
    /* Check if lexer detected an error */
    if (tokens->error_message) {
        if (!test_case->should_succeed) {
            printf("PASSED - Expected lexer error: %s\n", tokens->error_message);
            results->passed_tests++;
            free_tokens(tokens);
            return;
        } else {
            printf("FAILED - Unexpected lexer error: %s\n", tokens->error_message);
            results->failed_tests++;
            free_tokens(tokens);
            return;
        }
    }
    
    /* Step 2: Parser */
    t_parse_result *parse_result = parse(tokens);
    if (!parse_result) {
        printf("FAILED - Parser returned NULL\n");
        results->failed_tests++;
        free_tokens(tokens);
        return;
    }
    
    /* Check if parser detected an error */
    if (parse_result->error_msg) {
        if (!test_case->should_succeed) {
            printf("PASSED - Expected parser error: %s\n", parse_result->error_msg);
            results->passed_tests++;
            free_parse_result(parse_result);
            free_tokens(tokens);
            return;
        } else {
            printf("FAILED - Unexpected parser error: %s\n", parse_result->error_msg);
            results->failed_tests++;
            free_parse_result(parse_result);
            free_tokens(tokens);
            return;
        }
    }
    
    /* Step 3: Validate successful parsing */
    if (!test_case->should_succeed) {
        printf("FAILED - Expected error but parsing succeeded\n");
        results->failed_tests++;
        free_parse_result(parse_result);
        free_tokens(tokens);
        return;
    }
    
    /* Validate AST structure */
    t_pipeline *ast = parse_result->ast;
    if (!ast) {
        printf("FAILED - AST is NULL\n");
        results->failed_tests++;
        free_parse_result(parse_result);
        free_tokens(tokens);
        return;
    }
    
    /* Check pipeline count */
    int pipeline_count = count_pipelines(ast);
    if (test_case->expected_pipeline_count > 0 && pipeline_count != test_case->expected_pipeline_count) {
        printf("FAILED - Expected %d pipelines, got %d\n", 
               test_case->expected_pipeline_count, pipeline_count);
        results->failed_tests++;
        free_parse_result(parse_result);
        free_tokens(tokens);
        return;
    }
    
    /* Check command count in first pipeline */
    int cmd_count = count_commands_in_pipeline(ast);
    if (test_case->expected_cmd_count_in_first_pipeline > 0 && 
        cmd_count != test_case->expected_cmd_count_in_first_pipeline) {
        printf("FAILED - Expected %d commands in first pipeline, got %d\n", 
               test_case->expected_cmd_count_in_first_pipeline, cmd_count);
        results->failed_tests++;
        free_parse_result(parse_result);
        free_tokens(tokens);
        return;
    }
    
    /* Check first command name */
    if (test_case->expected_first_command) {
        t_cmd *first_cmd = ast->cmds;
        if (!first_cmd || !first_cmd->argv || !first_cmd->argv[0]) {
            printf("FAILED - First command is NULL or has no argv\n");
            results->failed_tests++;
            free_parse_result(parse_result);
            free_tokens(tokens);
            return;
        }
        
        if (strcmp(first_cmd->argv[0], test_case->expected_first_command) != 0) {
            printf("FAILED - Expected first command '%s', got '%s'\n", 
                   test_case->expected_first_command, first_cmd->argv[0]);
            results->failed_tests++;
            free_parse_result(parse_result);
            free_tokens(tokens);
            return;
        }
    }
    
    printf("PASSED\n");
    results->passed_tests++;
    
    /* Cleanup */
    free_parse_result(parse_result);
    free_tokens(tokens);
}

/* Main test suite */
int main(void)
{
    printf("=== Lexer-Parser Integration Tests ===\n\n");
    
    test_results results = {0, 0, 0};
    
    /* Test cases covering all subject requirements */
    integration_test_case test_cases[] = {
        /* Basic commands */
        {"ls", "Simple command", 1, 1, 1, "ls", NULL},
        {"echo hello", "Command with argument", 1, 1, 1, "echo", NULL},
        {"echo hello world", "Command with multiple arguments", 1, 1, 1, "echo", NULL},
        {"pwd", "Simple builtin command", 1, 1, 1, "pwd", NULL},
        {"cd /home", "Builtin with argument", 1, 1, 1, "cd", NULL},
        {"env", "Builtin without arguments", 1, 1, 1, "env", NULL},
        {"export VAR=value", "Export command with assignment", 1, 1, 1, "export", NULL},
        {"unset VAR", "Unset command", 1, 1, 1, "unset", NULL},
        {"exit", "Exit command", 1, 1, 1, "exit", NULL},
        {"exit 0", "Exit with status", 1, 1, 1, "exit", NULL},
        
        /* Pipes */
        {"ls | grep main", "Simple pipe", 1, 1, 2, "ls", NULL},
        {"cat file | head -n 5", "Pipe with arguments", 1, 1, 2, "cat", NULL},
        {"ps aux | grep bash | wc -l", "Multiple pipes", 1, 1, 3, "ps", NULL},
        
        /* Redirections */
        {"echo hello > file.txt", "Output redirection", 1, 1, 1, "echo", NULL},
        {"echo hello >> file.txt", "Append redirection", 1, 1, 1, "echo", NULL},
        {"cat < file.txt", "Input redirection", 1, 1, 1, "cat", NULL},
        {"cat << EOF", "Heredoc", 1, 1, 1, "cat", NULL},
        {"ls > file.txt 2>&1", "Output with error redirection", 1, 1, 1, "ls", NULL},
        
        /* Quotes */
        {"echo 'hello world'", "Single quotes", 1, 1, 1, "echo", NULL},
        {"echo \"hello world\"", "Double quotes", 1, 1, 1, "echo", NULL},
        {"echo 'don\\'t'", "Escaped single quote", 1, 1, 1, "echo", NULL},
        {"echo \"hello $USER\"", "Variable in double quotes", 1, 1, 1, "echo", NULL},
        {"echo '$USER'", "Variable in single quotes (no expansion)", 1, 1, 1, "echo", NULL},
        
        /* Environment variables */
        {"echo $HOME", "Environment variable", 1, 1, 1, "echo", NULL},
        {"echo $?", "Exit status variable", 1, 1, 1, "echo", NULL},
        {"echo $USER $HOME", "Multiple variables", 1, 1, 1, "echo", NULL},
        
        /* Operators (if lexer supports them) */
        {"echo hello && echo world", "AND operator", 1, 2, 1, "echo", NULL},
        {"false || echo world", "OR operator", 1, 2, 1, "false", NULL},
        {"echo hello; echo world", "Semicolon operator", 1, 2, 1, "echo", NULL},
        
        /* Complex combinations */
        {"ls -la | grep \".txt\" > files.txt", "Pipe with redirection", 1, 1, 2, "ls", NULL},
        {"echo \"$USER: $(date)\" >> log.txt", "Command substitution with redirection", 1, 1, 1, "echo", NULL},
        
        /* Error cases */
        {"", "Empty input", 0, 0, 0, NULL, NULL},
        {"|", "Pipe at start", 0, 0, 0, NULL, NULL},
        {"ls |", "Pipe at end", 0, 0, 0, NULL, NULL},
        {"ls > ", "Redirect without file", 0, 0, 0, NULL, NULL},
        {"ls < ", "Input redirect without file", 0, 0, 0, NULL, NULL},
        {"echo 'unclosed", "Unclosed single quote", 0, 0, 0, NULL, NULL},
        {"echo \"unclosed", "Unclosed double quote", 0, 0, 0, NULL, NULL},
        {"&&", "Operator without command", 0, 0, 0, NULL, NULL},
        {"||", "OR operator without command", 0, 0, 0, NULL, NULL},
        {";", "Semicolon without command", 0, 0, 0, NULL, NULL},
        
        /* Edge cases */
        {"   ls   ", "Command with whitespace", 1, 1, 1, "ls", NULL},
        {"ls\t-la", "Command with tab", 1, 1, 1, "ls", NULL},
        {"ls#comment", "Command with comment", 1, 1, 1, "ls", NULL},
    };
    
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    /* Run all tests */
    for (int i = 0; i < num_tests; i++) {
        run_integration_test(&test_cases[i], &results);
    }
    
    /* Print summary */
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", results.total_tests);
    printf("Passed: %d\n", results.passed_tests);
    printf("Failed: %d\n", results.failed_tests);
    printf("Success rate: %.1f%%\n", 
           (float)results.passed_tests / results.total_tests * 100);
    
    if (results.failed_tests > 0) {
        printf("\n⚠️  %d tests failed - bugs detected!\n", results.failed_tests);
        return 1;
    } else {
        printf("\n✅ All tests passed!\n");
        return 0;
    }
}