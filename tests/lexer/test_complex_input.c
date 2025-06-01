# include "tester_lexer.h"

// 複雑な入力のテスト
Test(lexer_test, complex_input) {
    // 複雑な入力のテスト
    expected_token complex_tokens[] = {
        {TOKEN_WORD, {"echo"}},
        {TOKEN_WORD, {"hello 'nested'"}},
        {TOKEN_EOF, {""}}
    };

    lexer_test_case complex_case = {
        "echo \"hello 'nested'\"",
        1,
        complex_tokens,
        (sizeof(complex_tokens) / sizeof(expected_token))
    };

    run_test_cases(&complex_case, 1, "complex");
}
