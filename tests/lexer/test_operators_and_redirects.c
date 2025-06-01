#include "tester_lexer.h"

// 他のトークンタイプのためのテストケース
Test(lexer_test, handle_operators_and_redirects) {
    expected_token pipe_token[] = {{TOKEN_PIPE, {"|"}},{TOKEN_EOF, {""}}};
    expected_token redirect_out_token[] = {{TOKEN_REDIRECT, {.redirect.redirect_type = REDIRECT_OUTPUT}},{TOKEN_EOF, {""}}};
    expected_token redirect_append_token[] = {{TOKEN_REDIRECT, {.redirect.redirect_type = REDIRECT_APPEND}},{TOKEN_EOF, {""}}};
    expected_token redirect_in_token[] = {{TOKEN_REDIRECT, {.redirect.redirect_type = REDIRECT_INPUT}},{TOKEN_EOF, {""}}};
    expected_token redirect_heredoc_token[] = {{TOKEN_REDIRECT, {.redirect.redirect_type = REDIRECT_HEREDOC}},{TOKEN_EOF, {""}}};
    // expected_token redirect_fd_out_token[] = {{TOKEN_REDIRECT, {"2>"}, 1},{TOKEN_EOF, {""}, 0}};
    // expected_token redirect_fd_dup_token[] = {{TOKEN_REDIRECT, {"2>&1"}, 1},{TOKEN_EOF, {""}, 0}};

    lexer_test_case cases[] = {
        {"|", 1, pipe_token, 2},
        {">", 1, redirect_out_token, 2},
        {">>", 1, redirect_append_token, 2},
        {"<", 1, redirect_in_token, 2},
        {"<<", 1, redirect_heredoc_token, 2},
        // {"2>", 1, redirect_fd_out_token, 2},
        // {"2>&1", 1, redirect_fd_dup_token, 2}
    };

    run_test_cases(cases, sizeof(cases) / sizeof(cases[0]), "operators");
}
