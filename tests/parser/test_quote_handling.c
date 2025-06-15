#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "adapters/parser/parser.h"
#include "domain/token.h"

/* テスト用のヘルパー関数（test_simple_command.cと同じ） */
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

/* テスト1: シングルクォートの処理 */
void test_single_quotes(void)
{
    printf("Test: single quoted string 'echo \"hello world\"'... ");

    // トークンストリームを作成
    // echo 'hello world' -> [WORD("echo"), WORD("hello world"), EOF]
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "hello world"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);
    assert(result->ast != NULL);

    // コマンドの検証
    t_cmd *cmd = result->ast->cmds;
    assert(cmd != NULL);
    assert(strcmp(cmd->argv[0], "echo") == 0);
    assert(strcmp(cmd->argv[1], "hello world") == 0);
    assert(cmd->argv[2] == NULL);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト2: ダブルクォートの処理 */
void test_double_quotes(void)
{
    printf("Test: double quoted string 'echo \"hello   world\"'... ");

    // トークンストリームを作成
    // echo "hello   world" -> [WORD("echo"), WORD("hello   world"), EOF]
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "hello   world"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // コマンドの検証
    t_cmd *cmd = result->ast->cmds;
    assert(strcmp(cmd->argv[0], "echo") == 0);
    assert(strcmp(cmd->argv[1], "hello   world") == 0);
    assert(cmd->argv[2] == NULL);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト3: 空のクォート */
void test_empty_quotes(void)
{
    printf("Test: empty quotes 'echo \"\" \\'\\''... ");

    // トークンストリームを作成
    // echo "" '' -> [WORD("echo"), WORD(""), WORD(""), EOF]
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, ""));
    add_token_to_stream(stream, create_token(TOKEN_WORD, ""));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // コマンドの検証
    t_cmd *cmd = result->ast->cmds;
    assert(strcmp(cmd->argv[0], "echo") == 0);
    assert(strcmp(cmd->argv[1], "") == 0);
    assert(strcmp(cmd->argv[2], "") == 0);
    assert(cmd->argv[3] == NULL);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト4: クォート内の特殊文字 */
void test_special_chars_in_quotes(void)
{
    printf("Test: special chars in quotes 'echo \\'$HOME | ls\\''... ");

    // トークンストリームを作成
    // echo '$HOME | ls' -> [WORD("echo"), WORD("$HOME | ls"), EOF]
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "echo"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "$HOME | ls"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // コマンドの検証
    t_cmd *cmd = result->ast->cmds;
    assert(strcmp(cmd->argv[0], "echo") == 0);
    assert(strcmp(cmd->argv[1], "$HOME | ls") == 0);
    assert(cmd->argv[2] == NULL);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* メイン関数 */
int main(void)
{
    printf("=== Parser Tests: Quote Handling ===\n");

    test_single_quotes();
    test_double_quotes();
    test_empty_quotes();
    test_special_chars_in_quotes();

    printf("\nAll tests passed!\n");
    return 0;
}
