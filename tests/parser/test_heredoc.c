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
    } else if (type == TOKEN_REDIRECT) {
        // リダイレクトのタイプを設定
        if (strcmp(text, "<<") == 0) {
            token->value.redirect.redirect_type = REDIRECT_HEREDOC;
            // token->value.redirect.n = -1;
        } else if (strcmp(text, ">") == 0) {
            token->value.redirect.redirect_type = REDIRECT_OUTPUT;
            // token->value.redirect.n = -1;
        }
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

/* テスト1: 基本的なヒアドキュメント */
void test_simple_heredoc(void)
{
    printf("Test: simple heredoc 'cat << EOF'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "cat"));
    add_token_to_stream(stream, create_token(TOKEN_REDIRECT, "<<"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "EOF"));
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
    assert(strcmp(cmd->argv[0], "cat") == 0);
    assert(cmd->argv[1] == NULL);

    // ヒアドキュメントの検証
    assert(cmd->redirects != NULL);
    assert(cmd->redirects->type == REDIRECT_HEREDOC);
    assert(strcmp(cmd->redirects->file, "EOF") == 0);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト2: コマンド引数とヒアドキュメント */
void test_heredoc_with_args(void)
{
    printf("Test: heredoc with args 'grep pattern << END'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "grep"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "pattern"));
    add_token_to_stream(stream, create_token(TOKEN_REDIRECT, "<<"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "END"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // コマンドとヒアドキュメントの検証
    t_cmd *cmd = result->ast->cmds;
    assert(strcmp(cmd->argv[0], "grep") == 0);
    assert(strcmp(cmd->argv[1], "pattern") == 0);
    assert(cmd->redirects != NULL);
    assert(cmd->redirects->type == REDIRECT_HEREDOC);
    assert(strcmp(cmd->redirects->file, "END") == 0);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト3: 複数のリダイレクションとヒアドキュメント */
void test_heredoc_with_multiple_redirections(void)
{
    printf("Test: heredoc with redirections 'cmd << EOF > out.txt'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "cmd"));
    add_token_to_stream(stream, create_token(TOKEN_REDIRECT, "<<"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "EOF"));
    add_token_to_stream(stream, create_token(TOKEN_REDIRECT, ">"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "out.txt"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // 最初のリダイレクション（ヒアドキュメント）
    t_cmd *cmd = result->ast->cmds;
    assert(cmd->redirects != NULL);
    assert(cmd->redirects->type == REDIRECT_HEREDOC);
    assert(strcmp(cmd->redirects->file, "EOF") == 0);

    // 2番目のリダイレクション（出力）
    assert(cmd->redirects->next != NULL);
    assert(cmd->redirects->next->type == REDIRECT_OUTPUT);
    assert(strcmp(cmd->redirects->next->file, "out.txt") == 0);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト4: パイプとヒアドキュメント */
void test_heredoc_with_pipe(void)
{
    printf("Test: heredoc with pipe 'cat << EOF | grep pattern'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "cat"));
    add_token_to_stream(stream, create_token(TOKEN_REDIRECT, "<<"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "EOF"));
    add_token_to_stream(stream, create_token(TOKEN_PIPE, "|"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "grep"));
    add_token_to_stream(stream, create_token(TOKEN_WORD, "pattern"));
    add_token_to_stream(stream, create_token(TOKEN_EOF, ""));

    // パース実行
    t_parse_result *result = parse(stream);

    // 結果の検証
    assert(result != NULL);
    assert(result->error_msg == NULL);

    // 最初のコマンド（cat << EOF）
    t_cmd *cmd1 = result->ast->cmds;
    assert(strcmp(cmd1->argv[0], "cat") == 0);
    assert(cmd1->redirects != NULL);
    assert(cmd1->redirects->type == REDIRECT_HEREDOC);
    assert(strcmp(cmd1->redirects->file, "EOF") == 0);

    // 2番目のコマンド（grep pattern）
    t_cmd *cmd2 = cmd1->next;
    assert(cmd2 != NULL);
    assert(strcmp(cmd2->argv[0], "grep") == 0);
    assert(strcmp(cmd2->argv[1], "pattern") == 0);

    // クリーンアップ
    free_parse_result(result);
    free_token_stream(stream);

    printf("PASSED\n");
}

/* テスト5: ヒアドキュメントエラー（区切り文字なし） */
void test_heredoc_without_delimiter(void)
{
    printf("Test: heredoc without delimiter 'cat <<'... ");

    // トークンストリームを作成
    t_token_stream *stream = create_token_stream();
    add_token_to_stream(stream, create_token(TOKEN_WORD, "cat"));
    add_token_to_stream(stream, create_token(TOKEN_REDIRECT, "<<"));
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
    printf("=== Parser Tests: Heredoc Handling ===\n");

    test_simple_heredoc();
    test_heredoc_with_args();
    test_heredoc_with_multiple_redirections();
    test_heredoc_with_pipe();
    test_heredoc_without_delimiter();

    printf("\nAll tests passed!\n");
    return 0;
}
