# include "tester_lexer.h"

// エラー処理のテスト
Test(lexer_test, error_handling) {
    lexer_test_case cases[] = {
        {"'unterminated", 0, NULL, 0},
        {"\"unterminated double quote", 0, NULL, 0}
        // 他のエラーケース
    };

    run_test_cases(cases, sizeof(cases) / sizeof(cases[0]), "errors");
}
