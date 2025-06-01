# include "tester_lexer.h"

// 変数代入のテスト
Test(lexer_test, handle_assignments) {
    // 期待されるトークンの定義
    expected_token var_value_token[] = {
        {TOKEN_ASSIGNMENT, {.assignment = {.name = "VAR",.operator="=", .value = "value", .text = "VAR=value"}}},
        {TOKEN_EOF, {""}}
    };
    expected_token v1ar_value_token[] = {
        {TOKEN_ASSIGNMENT, {.assignment = {.name = "V1AR", .operator="=", .value = "value", .text = "V1AR=value"}}},
        {TOKEN_EOF, {""}}
    };
    expected_token path_token[] = {
        {TOKEN_ASSIGNMENT, {.assignment = {.name = "PATH", .operator="=", .value = "/usr/bin", .text = "PATH=/usr/bin"}}},
        {TOKEN_EOF, {""}}
    };
    expected_token x_empty_token[] = {
        {TOKEN_ASSIGNMENT, {.assignment = {.name = "X", .operator="=", .value = "", .text = "X="}}},
        {TOKEN_EOF, {""}}
    };

    // 複数の変数代入を含むテスト
    expected_token multiple_assignments[] = {
        {TOKEN_ASSIGNMENT, {.assignment = {.name = "X", .operator="=", .value = "1;", .text = "X=1;"}}},
        {TOKEN_ASSIGNMENT, {.assignment = {.name = "Y", .operator="=", .value = "2", .text = "Y=2"}}},
        {TOKEN_EOF, {""}}
    };
    // += 演算子を含む変数代入のテスト
    expected_token plus_assignments[] = {
        {TOKEN_ASSIGNMENT, {.assignment = {.name = "X", .operator="=", .value = "1", .text = "X=1"}}},
        {TOKEN_ASSIGNMENT, {.assignment = {.name = "X", .operator="+=", .value = "2", .text = "X+=2"}}},
        {TOKEN_EOF, {""}}
    };

        // 複数の変数代入を含むテスト
    expected_token invalid_assignments[] = {
        {TOKEN_WORD, {"=value"}},
        {TOKEN_EOF, {""}}
    };

    lexer_test_case cases[] = {
        {"VAR=value", 1, var_value_token, 2},
        {"V1AR=value", 1, v1ar_value_token, 2},
        {"PATH=/usr/bin", 1, path_token, 2},
        {"X=", 1, x_empty_token, 2},
        {"X=1; Y=2", 1, multiple_assignments, 3},
        {"X=1 X+=2", 1, plus_assignments, 3},
        {"=value", 1, invalid_assignments, 2}
    };

    run_test_cases(cases, sizeof(cases) / sizeof(cases[0]), "assignments");
}
