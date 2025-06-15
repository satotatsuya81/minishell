#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usecase/lexer/lexer.h"
#include "adapters/parser/parser.h"
#include "domain/token.h"

/* Simple test function to verify lexer-parser integration */
int test_simple_command(const char *input, const char *expected_cmd)
{
    printf("Testing: '%s' -> ", input);
    
    /* Step 1: Lexer */
    t_token_stream *tokens = lexer(input);
    if (!tokens) {
        printf("FAILED - Lexer returned NULL\n");
        return 0;
    }
    
    if (tokens->error_message) {
        printf("FAILED - Lexer error: %s\n", tokens->error_message);
        free_tokens(tokens);
        return 0;
    }
    
    /* Step 2: Parser */
    t_parse_result *result = parse(tokens);
    if (!result) {
        printf("FAILED - Parser returned NULL\n");
        free_tokens(tokens);
        return 0;
    }
    
    if (result->error_msg) {
        printf("FAILED - Parser error: %s\n", result->error_msg);
        free_parse_result(result);
        free_tokens(tokens);
        return 0;
    }
    
    /* Step 3: Validate */
    if (!result->ast || !result->ast->cmds || !result->ast->cmds->argv || !result->ast->cmds->argv[0]) {
        printf("FAILED - Invalid AST structure\n");
        free_parse_result(result);
        free_tokens(tokens);
        return 0;
    }
    
    if (strcmp(result->ast->cmds->argv[0], expected_cmd) != 0) {
        printf("FAILED - Expected '%s', got '%s'\n", expected_cmd, result->ast->cmds->argv[0]);
        free_parse_result(result);
        free_tokens(tokens);
        return 0;
    }
    
    printf("PASSED\n");
    
    /* Cleanup */
    free_parse_result(result);
    free_tokens(tokens);
    return 1;
}

int main(void)
{
    int passed = 0;
    int total = 0;
    
    printf("=== Simple Lexer-Parser Integration Test ===\n");
    
    /* Basic command tests */
    total++; passed += test_simple_command("ls", "ls");
    total++; passed += test_simple_command("echo hello", "echo");
    total++; passed += test_simple_command("pwd", "pwd");
    total++; passed += test_simple_command("cat file.txt", "cat");
    
    /* Test with quotes */
    total++; passed += test_simple_command("echo 'hello world'", "echo");
    total++; passed += test_simple_command("echo \"hello world\"", "echo");
    
    /* Test with spaces */
    total++; passed += test_simple_command("   ls   ", "ls");
    
    printf("\n=== Summary ===\n");
    printf("Passed: %d/%d\n", passed, total);
    
    if (passed == total) {
        printf("✅ All basic tests passed!\n");
        return 0;
    } else {
        printf("❌ %d tests failed\n", total - passed);
        return 1;
    }
}