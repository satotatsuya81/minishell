#include "tester_lexer.h"
#include <criterion/assert.h>
/**
 * 共通のテスト実行関数
 *
 * @param cases テストケースの配列
 * @param case_count テストケースの数
 * @param test_name テストの名前（ログ用）
 */
void run_test_cases(const lexer_test_case *cases, size_t case_count, const char *test_name) {
    for (size_t i = 0; i < case_count; ++i) {
        printf("Running %s test %zu: input = \"%s\"\n", test_name, i, cases[i].input);

        // レキサーを呼び出し
        t_token_stream *stream = lexer(cases[i].input);

        // 結果の検証
        if (cases[i].expected_success) {
            // 成功が期待される場合
            cr_assert_not_null(stream,
                             "%s test %zu: Expected stream not to be NULL for input \"%s\"",
                             test_name, i, cases[i].input);
            cr_assert_not_null(stream->head,
                             "%s test %zu: Expected token stream to have tokens for input \"%s\"",
                             test_name, i, cases[i].input);

            // 期待されるトークン数の検証
            int token_count = 0;
            t_token *current = stream->head;

            // 各トークンの検証
            while (current != NULL && token_count < (int)cases[i].token_count - 1) {
                expected_token expected = cases[i].expected_tokens[token_count];

                // トークンタイプの検証
                cr_expect_eq(current->type, expected.type,
                           "%s test %zu: t_token %d - Expected type %d but got %d",
                           test_name, i, token_count, expected.type, current->type);
				// トークンタイプに応じた値の検証
				if (current->type == TOKEN_WORD) {
					cr_expect_str_eq(current->value.word, expected.value.word,
								  "%s test %zu: t_token %d - Expected word \"%s\" but got \"%s\"",
								  test_name, i, token_count, expected.value.word, current->value.word);
				} else if (current->type == TOKEN_ASSIGNMENT) {
					cr_expect_not_null(current->value.assignment.name,
									"%s test %zu: t_token %d - Assignment name should not be NULL",
									test_name, i, token_count);
					cr_expect_str_eq(current->value.assignment.name, expected.value.assignment.name,
								  "%s test %zu: t_token %d - Expected assignment name \"%s\" but got \"%s\"",
								  test_name, i, token_count, expected.value.assignment.name,
								  current->value.assignment.name);
					cr_expect_str_eq(current->value.assignment.value, expected.value.assignment.value,
								  "%s test %zu: t_token %d - Expected assignment value \"%s\" but got \"%s\"",
								  test_name, i, token_count, expected.value.assignment.value,
								  current->value.assignment.value);
					cr_expect_str_eq(current->value.assignment.operator,
							   expected.value.assignment.operator,
							   "%s test %zu: t_token %d - Expected assignment operator \"%s\" but got \"%s\"",
							   test_name, i, token_count, expected.value.assignment.operator,
							   current->value.assignment.operator);
					cr_expect_str_eq(current->value.assignment.text, expected.value.assignment.text,
								  "%s test %zu: t_token %d - Expected assignment text \"%s\" but got \"%s\"",
								  test_name, i, token_count, expected.value.assignment.text,
								  current->value.assignment.text);
				} else if (current->type == TOKEN_REDIRECT) {
					cr_expect_eq(current->value.redirect.redirect_type, expected.value.redirect.redirect_type,
							   "%s test %zu: t_token %d - Expected redirect type %d but got %d",
							   test_name, i, token_count, expected.value.redirect.redirect_type,
							   current->value.redirect.redirect_type);
				}
				current = current->next;
				token_count++;
			}

			// EOFのトークンが期待される場合
			cr_expect_eq(current->type, TOKEN_EOF,
					   "%s test %zu: Expected EOF token but got type %d",
					   test_name, i, current->type);
			current = current->next;
			token_count++;
			// 期待される数のトークンが検出されたか確認
			cr_expect_eq(token_count, (int)cases[i].token_count,
			"%s test %zu: Expected %zu tokens but got %d",
			test_name, i, cases[i].token_count, token_count);

			// 余分なトークンがないことを確認
			cr_expect_null(current,
				"%s test %zu: Unexpected extra tokens after expected tokens",
				test_name, i);
			printf("Completed %s test %zu: input = \"%s\"\n", test_name, i, cases[i].input);
		} else {
			// エラーが期待される場合
			if (stream != NULL) {
				cr_expect_not_null(stream->error_message,
								"%s test %zu: Expected error message for input \"%s\"",
								test_name, i, cases[i].input);

				// エラー位置の情報が設定されていることを確認
				cr_expect_gt(stream->error_line, 0,
						"%s test %zu: Expected error_line to be set", test_name, i);
				cr_expect_gt(stream->error_column, 0,
						"%s test %zu: Expected error_column to be set", test_name, i);
			}
		}
		// ストリームのクリーンアップ (lexer関数がメモリを確保する場合は必要)
		free_tokens(stream);
	}
}
