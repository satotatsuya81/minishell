#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "usecase/parser/parser.h"
#include "domain/token.h"

/* テスト用のヘルパー関数 */
static t_token *create_token(t_token_type type, const char *text)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;

    token->type = type;
    // token->text = strdup(text);
    token->length = strlen(text);
    token->line_number = 1;
    token->column = 1;
    token->next = NULL;

    if (type == TOKEN_WORD) {
        token->value.word = strdup(text);
    }

    return token;
}

static t_token_stream *create_token_stream(void)
{
    t_token_stream *stream = malloc(sizeof(t_token_stream));
    if (!stream)
        return NULL;

    stream->head = NULL;
    stream->current = NULL;
    stream->count = 0;
    stream->error_message = NULL;
    stream->error_line = 0;
    stream->error_column = 0;

    return stream;
}

static void add_token_to_stream(t_token_stream *stream, t_token *token)
{
    if (!stream->head) {
        stream->head = token;
        stream->current = token;
    } else {
        t_token *last = stream->head;
        while (last->next)
            last = last->next;
        last->next = token;
    }
    stream->count++;
}

static void free_token_stream(t_token_stream *stream)
{
    if (!stream)
        return;

    t_token *current = stream->head;
    while (current) {
        t_token *next = current->next;
        // free(current->text);
        if (current->type == TOKEN_WORD)
            free(current->value.word);
        free(current);
        current = next;
    }

    free(stream->error_message);
    free(stream);
}

/* テスト1: AND演算子 */
void test_and_operator(void)
{
    printf("Test: AND operator 'echo ok && ls'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    t_token *op_token = create_token(TOKEN_OPERATOR, "&&");
    op_token->value.word = strdup("&&");

    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "ok"));
    add_token_to_stream(stream, op_token);
    add_token_to_stream(stream, create_token(TOKEN_WORD, "ls"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);
    assert(result->ast != NULL);

    // 最初のパイプライン
    t_pipeline *pipeline1 = result->ast;
    assert(pipeline1 != NULL);
    assert(pipeline1->cmds != NULL);
    assert(strcmp(pipeline1->cmds->argv[0], "echo") == 0);
    assert(strcmp(pipeline1->cmds->argv[1], "ok") == 0);
    assert(pipeline1->connector == CONN_AND);

    // 2番目のパイプライン
    t_pipeline *pipeline2 = pipeline1->next;
    assert(pipeline2 != NULL);
    assert(pipeline2->cmds != NULL);
    assert(strcmp(pipeline2->cmds->argv[0], "ls") == 0);
    assert(pipeline2->next == NULL);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト2: OR演算子 */
void test_or_operator(void)
{
    printf("Test: OR operator 'false || echo error'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    t_token *op_token = create_token(TOKEN_OPERATOR, "||");
    op_token->value.word = strdup("||");

    add_token_to_stream(stream, create_token(TOKEN_WORD, "false"));
    add_token_to_stream(stream, op_token);
    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "error"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // パイプラインの検証
    t_pipeline *pipeline1 = result->ast;
    assert(strcmp(pipeline1->cmds->argv[0], "false") == 0);
    assert(pipeline1->connector == CONN_OR);

    t_pipeline *pipeline2 = pipeline1->next;
    assert(pipeline2 != NULL);
    assert(strcmp(pipeline2->cmds->argv[0], "echo") == 0);
    assert(strcmp(pipeline2->cmds->argv[1], "error") == 0);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト3: セミコロン演算子 */
void test_semicolon_operator(void)
{
    printf("Test: semicolon operator 'echo first ; echo second'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    t_token *op_token = create_token(TOKEN_OPERATOR, ";");
    op_token->value.word = strdup(";");

    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "first"));
    add_token_to_stream(stream, op_token);
    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "second"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // パイプラインの検証
    t_pipeline *pipeline1 = result->ast;
    assert(strcmp(pipeline1->cmds->argv[0], "echo") == 0);
    assert(strcmp(pipeline1->cmds->argv[1], "first") == 0);
    assert(pipeline1->connector == CONN_SEMICOLON);

    t_pipeline *pipeline2 = pipeline1->next;
    assert(strcmp(pipeline2->cmds->argv[0], "echo") == 0);
    assert(strcmp(pipeline2->cmds->argv[1], "second") == 0);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト4: 複数の演算子の組み合わせ */
void test_mixed_operators(void)
{
    printf("Test: mixed operators 'cmd1 && cmd2 || cmd3 ; cmd4'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    t_token *and_token = create_token(TOKEN_OPERATOR, "&&");
    and_token->value.word = strdup("&&");
    t_token *or_token = create_token(TOKEN_OPERATOR, "||");
    or_token->value.word = strdup("||");
    t_token *semi_token = create_token(TOKEN_OPERATOR, ";");
    semi_token->value.word = strdup(";");

    add_token_to_stream(stream, create_token(TOKEN_WORD, "cmd1"));
    add_token_to_stream(stream, and_token);
    add_token_to_stream(stream, create_token(TOKEN_WORD, "cmd2"));
    add_token_to_stream(stream, or_token);
    add_token_to_stream(stream, create_token(TOKEN_WORD, "cmd3"));
    add_token_to_stream(stream, semi_token);
    add_token_to_stream(stream, create_token(TOKEN_WORD, "cmd4"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // パイプラインチェーンの検証
    t_pipeline *p1 = result->ast;
    assert(strcmp(p1->cmds->argv[0], "cmd1") == 0);
    assert(p1->connector == CONN_AND);

    t_pipeline *p2 = p1->next;
    assert(strcmp(p2->cmds->argv[0], "cmd2") == 0);
    assert(p2->connector == CONN_OR);

    t_pipeline *p3 = p2->next;
    assert(strcmp(p3->cmds->argv[0], "cmd3") == 0);
    assert(p3->connector == CONN_SEMICOLON);

    t_pipeline *p4 = p3->next;
    assert(strcmp(p4->cmds->argv[0], "cmd4") == 0);
    assert(p4->next == NULL);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト5: 演算子とパイプの組み合わせ */
void test_operators_with_pipes(void)
{
    printf("Test: operators with pipes 'ls | grep txt && echo found'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    t_token *and_token = create_token(TOKEN_OPERATOR, "&&");
    and_token->value.word = strdup("&&");

    add_token_to_stream(stream, create_token(TOKEN_WORD, "ls"));
    add_token_to_stream(stream, create_token(TOKEN_PIPE, "|"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "grep"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "txt"));
    add_token_to_stream(stream, and_token);
    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "found"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // 最初のパイプライン (ls | grep txt)
    t_pipeline *p1 = result->ast;
    assert(p1->cmds != NULL);
    assert(strcmp(p1->cmds->argv[0], "ls") == 0);
    assert(p1->cmds->next != NULL);
    assert(strcmp(p1->cmds->next->argv[0], "grep") == 0);
    assert(p1->connector == CONN_AND);

    // 2番目のパイプライン (echo found)
    t_pipeline *p2 = p1->next;
    assert(strcmp(p2->cmds->argv[0], "echo") == 0);
    assert(strcmp(p2->cmds->argv[1], "found") == 0);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト6: 演算子エラー（演算子の後にコマンドがない） */
void test_operator_without_command(void)
{
    printf("Test: operator without command 'echo &&'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    t_token *op_token = create_token(TOKEN_OPERATOR, "&&");
    op_token->value.word = strdup("&&");

    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, op_token);
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // エラーが発生することを確認
    assert(result != NULL);
    assert(result->error_msg != NULL);
    assert(result->ast == NULL);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* メイン関数 */
int main(void)
{
    printf("=== Parser Tests: Operator Handling ===\n");

    test_and_operator();
    test_or_operator();
    test_semicolon_operator();
    test_mixed_operators();
    test_operators_with_pipes();
    test_operator_without_command();

    printf("\nAll tests passed!\n");
    return 0;
}
