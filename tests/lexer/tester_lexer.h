/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_lexer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:35:22 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 14:43:28 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTER_LEXER_H
# define TESTER_LEXER_H
# include <stdio.h>
# include <criterion/criterion.h>
# include "usecase/lexer/lexer.h"
# include "domain/token.h"
# include "usecase/lexer/token_type_handler.h"

// // 予想されるトークン構造体
// typedef struct {
//     t_token_type type;           // トークンタイプ
//     const char *value;           // 期待される値
//     char check_text_field;       // 1: text フィールドをチェック, 0: value.word フィールドをチェック
// } expected_token;

# define TEXT_FIELD_CHECK 1
# define VALUE_FIELD_CHECK 0
typedef struct {
    t_token_type type;           // トークンタイプ
    union {
        char *word;              // WORDトークンの値
        t_assignment assignment; // ASSIGNMENTトークンの値
        t_redirect redirect;     // REDIRECTトークンの値
    } value;                     // トークンの値
} expected_token;
// テストケース構造体の定義
typedef struct {
    const char *input;                 // 入力文字列
    int expected_success;              // 期待される結果 (1: 成功, 0: エラー)
    expected_token *expected_tokens;   // 期待されるトークンの配列
    size_t token_count;                // 期待されるトークンの数
} lexer_test_case;

void run_test_cases(const lexer_test_case *cases, size_t case_count, const char *test_name);

#endif
