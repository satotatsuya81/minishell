# include "tester_lexer.h"

Test(lexer_test, handle_quotes) {
    // シングルクォートのテスト
    expected_token hello_tokens[] = {
        {TOKEN_WORD, {"hello"}},
        {TOKEN_EOF, {""}}
    };

    // 'a b c' は単一トークン (シングルクォート内はスペースで分割されない)
    expected_token a_b_c_tokens[] = {
        {TOKEN_WORD, {"a b c"}},
        {TOKEN_EOF, {""}}
    };

    // 複数ワードのテスト（シングルクォート外ではスペースで分割）
    expected_token multiple_words_tokens[] = {
        {TOKEN_WORD, {"hello"}},
        {TOKEN_WORD, {"world"}},
        {TOKEN_EOF, {""}}
    };

    // ダブルクォートのテスト
    expected_token double_quoted_tokens[] = {
        {TOKEN_WORD, {"double quoted"}},
        {TOKEN_EOF, {""}}
    };

    // ダブルクォートのテスト
    expected_token quoted_pipe_tokens[] = {
        {TOKEN_WORD, {"| is not pipe"}},
        {TOKEN_EOF, {""}}
    };

    // 空文字列のテスト
    expected_token empty_tokens[] = {
        {TOKEN_WORD, {""}},
        {TOKEN_EOF, {""}}
    };

    // テストケースの定義
    lexer_test_case cases[] = {
        {"'hello'", 1, hello_tokens, 2},
        {"'a b c'", 1, a_b_c_tokens, 2},
        {"'hello' world", 1, multiple_words_tokens, 3},
        {"\"double quoted\"", 1, double_quoted_tokens, 2},
        {"''", 1, empty_tokens, 2},
        {"\"| is not pipe\"", 1, quoted_pipe_tokens, 2},
        {"'| is not pipe'", 1, quoted_pipe_tokens, 2},
        {"'unterminated", 0, NULL, 0}
    };

    run_test_cases(cases, sizeof(cases) / sizeof(cases[0]), "quotes");
}
