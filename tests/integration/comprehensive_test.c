#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usecase/lexer/lexer.h"
#include "usecase/parser/parser.h"
#include "domain/token.h"

typedef struct {
    const char *input;
    const char *description;
    int should_succeed;
    const char *expected_cmd;
    int expected_argc;
    int expected_pipe_count;
} test_case;

/* Count pipes in AST */
int count_pipes(t_pipeline *ast)
{
    if (!ast || !ast->cmds)
        return 0;
    
    int count = 0;
    t_cmd *cmd = ast->cmds;
    while (cmd) {
        if (cmd->next)
            count++;
        cmd = cmd->next;
    }
    return count;
}

/* Count arguments in first command */
int count_args(t_pipeline *ast)
{
    if (!ast || !ast->cmds || !ast->cmds->argv)
        return 0;
    
    int count = 0;
    char **argv = ast->cmds->argv;
    while (argv[count])
        count++;
    return count;
}

/* Run a comprehensive test */
int run_test(const test_case *tc)
{
    printf("Testing: %s -> ", tc->description);
    
    /* Step 1: Lexer */
    t_token_stream *tokens = lexer(tc->input);
    if (!tokens) {
        printf("FAILED - Lexer returned NULL\n");
        return 0;
    }
    
    if (tokens->error_message) {
        if (!tc->should_succeed) {
            printf("PASSED - Expected lexer error: %s\n", tokens->error_message);
            free_tokens(tokens);
            return 1;
        } else {
            printf("FAILED - Unexpected lexer error: %s\n", tokens->error_message);
            free_tokens(tokens);
            return 0;
        }
    }
    
    /* Step 2: Parser */
    t_parse_result *result = parse(tokens);
    if (!result) {
        printf("FAILED - Parser returned NULL\n");
        free_tokens(tokens);
        return 0;
    }
    
    if (result->error_msg) {
        if (!tc->should_succeed) {
            printf("PASSED - Expected parser error: %s\n", result->error_msg);
            free_parse_result(result);
            free_tokens(tokens);
            return 1;
        } else {
            printf("FAILED - Unexpected parser error: %s\n", result->error_msg);
            free_parse_result(result);
            free_tokens(tokens);
            return 0;
        }
    }
    
    /* Step 3: Validate successful parsing */
    if (!tc->should_succeed) {
        printf("FAILED - Expected error but parsing succeeded\n");
        free_parse_result(result);
        free_tokens(tokens);
        return 0;
    }
    
    if (!result->ast) {
        printf("FAILED - AST is NULL\n");
        free_parse_result(result);
        free_tokens(tokens);
        return 0;
    }
    
    /* Check expected command */
    if (tc->expected_cmd) {
        if (!result->ast->cmds || !result->ast->cmds->argv || !result->ast->cmds->argv[0]) {
            printf("FAILED - First command is NULL\n");
            free_parse_result(result);
            free_tokens(tokens);
            return 0;
        }
        
        if (strcmp(result->ast->cmds->argv[0], tc->expected_cmd) != 0) {
            printf("FAILED - Expected cmd '%s', got '%s'\n", 
                   tc->expected_cmd, result->ast->cmds->argv[0]);
            free_parse_result(result);
            free_tokens(tokens);
            return 0;
        }
    }
    
    /* Check argument count */
    if (tc->expected_argc > 0) {
        int argc = count_args(result->ast);
        if (argc != tc->expected_argc) {
            printf("FAILED - Expected %d args, got %d\n", tc->expected_argc, argc);
            free_parse_result(result);
            free_tokens(tokens);
            return 0;
        }
    }
    
    /* Check pipe count */
    if (tc->expected_pipe_count >= 0) {
        int pipe_count = count_pipes(result->ast);
        if (pipe_count != tc->expected_pipe_count) {
            printf("FAILED - Expected %d pipes, got %d\n", tc->expected_pipe_count, pipe_count);
            free_parse_result(result);
            free_tokens(tokens);
            return 0;
        }
    }
    
    printf("PASSED\n");
    
    /* Cleanup */
    free_parse_result(result);
    free_tokens(tokens);
    return 1;
}

int main(void)
{
    printf("=== Comprehensive Lexer-Parser Integration Test ===\n\n");
    
    test_case tests[] = {
        /* Basic commands */
        {"ls", "Simple command", 1, "ls", 1, 0},
        {"echo hello", "Command with argument", 1, "echo", 2, 0},
        {"echo hello world", "Multiple arguments", 1, "echo", 3, 0},
        {"pwd", "Simple builtin", 1, "pwd", 1, 0},
        {"cd /home", "Builtin with path", 1, "cd", 2, 0},
        {"env", "Env builtin", 1, "env", 1, 0},
        {"exit", "Exit builtin", 1, "exit", 1, 0},
        {"exit 0", "Exit with status", 1, "exit", 2, 0},
        
        /* Pipes */
        {"ls | grep main", "Simple pipe", 1, "ls", 1, 1},
        {"cat file | head -n 5", "Pipe with args", 1, "cat", 2, 1},
        {"ps aux | grep bash | wc -l", "Multiple pipes", 1, "ps", 2, 2},
        {"echo hello | cat", "Simple pipe", 1, "echo", 2, 1},
        
        /* Redirections */
        {"echo hello > file.txt", "Output redirect", 1, "echo", 2, 0},
        {"echo hello >> file.txt", "Append redirect", 1, "echo", 2, 0},
        {"cat < file.txt", "Input redirect", 1, "cat", 1, 0},
        {"cat << EOF", "Heredoc", 1, "cat", 1, 0},
        
        /* Quotes */
        {"echo 'hello world'", "Single quotes", 1, "echo", 2, 0},
        {"echo \"hello world\"", "Double quotes", 1, "echo", 2, 0},
        {"echo 'don\\'t'", "Escaped quote", 1, "echo", 2, 0},
        
        /* Environment variables (may not work if lexer doesn't handle them) */
        {"echo $HOME", "Environment var", 1, "echo", 2, 0},
        {"echo $?", "Exit status var", 1, "echo", 2, 0},
        
        /* Operators (testing if lexer supports them) */
        {"echo hello && echo world", "AND operator", 1, "echo", 2, 0},
        {"false || echo world", "OR operator", 1, "false", 1, 0},
        {"echo hello; echo world", "Semicolon", 1, "echo", 2, 0},
        
        /* Complex cases */
        {"ls -la | grep \".txt\" > files.txt", "Pipe + redirect", 1, "ls", 2, 1},
        
        /* Edge cases */
        {"   ls   ", "Whitespace around", 1, "ls", 1, 0},
        {"ls\t-la", "Tab separator", 1, "ls", 2, 0},
        
        /* Error cases */
        {"", "Empty input", 0, NULL, 0, 0},
        {"|", "Pipe at start", 0, NULL, 0, 0},
        {"ls |", "Pipe at end", 0, NULL, 0, 0},
        {"ls > ", "Redirect no file", 0, NULL, 0, 0},
        {"echo 'unclosed", "Unclosed quote", 0, NULL, 0, 0},
        {"echo \"unclosed", "Unclosed dquote", 0, NULL, 0, 0},
        {"&&", "Only operator", 0, NULL, 0, 0},
        {"||", "Only OR", 0, NULL, 0, 0},
        {";", "Only semicolon", 0, NULL, 0, 0},
    };
    
    int total = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    int bugs_found = 0;
    
    printf("Running %d test cases...\n\n", total);
    
    for (int i = 0; i < total; i++) {
        if (run_test(&tests[i])) {
            passed++;
        } else {
            bugs_found++;
        }
    }
    
    printf("\n=== Test Results ===\n");
    printf("Total tests: %d\n", total);
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", total - passed);
    printf("Success rate: %.1f%%\n", (float)passed / total * 100);
    
    if (bugs_found > 0) {
        printf("\n🐛 %d potential bugs detected!\n", bugs_found);
        printf("\nBug categories to investigate:\n");
        printf("1. Lexer issues (tokenization failures)\n");
        printf("2. Parser issues (AST construction failures)\n");
        printf("3. Error handling (missing validation)\n");
        printf("4. Feature gaps (unsupported syntax)\n");
        return 1;
    } else {
        printf("\n✅ No bugs detected in basic functionality!\n");
        return 0;
    }
}