#include <stdlib.h>
#include "usecase/parser/parser.h"
#include "../libft/libft.h"

/* ヘルパー関数: 新しいパース結果を作成 */
static t_parse_result	*create_parse_result(void)
{
	t_parse_result *result = malloc(sizeof(t_parse_result));
	if (!result)
		return NULL;

	result->ast = NULL;
	result->error_msg = NULL;
	result->error_line = 0;
	result->error_column = 0;

	return result;
}

/* ヘルパー関数: 新しいパイプラインを作成 */
static t_pipeline	*create_pipeline(void)
{
	t_pipeline *pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return NULL;

	pipeline->cmds = NULL;
	pipeline->next = NULL;
	pipeline->connector = CONN_NONE;

	return pipeline;
}

/* ヘルパー関数: 新しいコマンドを作成 */
static t_cmd	*create_cmd(void)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return NULL;

	cmd->argv = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;

	return cmd;
}

/* ヘルパー関数: argv配列を拡張 */
static char **extend_argv(char **argv, int current_size)
{
	char **new_argv = malloc(sizeof(char *) * (current_size + 2));
	if (!new_argv)
		return NULL;

	for (int i = 0; i < current_size; i++) {
		new_argv[i] = argv[i];
	}
	new_argv[current_size] = NULL;
	new_argv[current_size + 1] = NULL;

	free(argv);
	return new_argv;
}

/* ヘルパー関数: 新しいリダイレクトを作成 */
static t_redirect_list *create_redirect(t_redirect_type type, const char *file)
{
	t_redirect_list *redirect = malloc(sizeof(t_redirect_list));
	if (!redirect)
		return NULL;

	redirect->type = type;
	redirect->file = ft_strdup(file);
	if (!redirect->file) {
		free(redirect);
		return NULL;
	}
	redirect->fd = -1;  // デフォルトのファイルディスクリプタ
	redirect->next = NULL;

	return redirect;
}

/* リダイレクションのパース */
static t_redirect_list *parse_redirection(t_token_stream *tokens)
{
	if (!tokens->current || tokens->current->type != TOKEN_REDIRECT)
		return NULL;

	// リダイレクトのタイプを取得
	t_redirect_type type = tokens->current->value.redirect.redirect_type;

	// リダイレクトトークンを消費
	tokens->current = tokens->current->next;

	// ファイル名が必要
	if (!tokens->current || tokens->current->type != TOKEN_WORD) {
		return NULL;
	}

	// リダイレクトを作成
	t_redirect_list *redirect = create_redirect(type, tokens->current->value.word);
	if (!redirect)
		return NULL;

	// ファイル名トークンを消費
	tokens->current = tokens->current->next;

	return redirect;
}

/* 単純なコマンドのパース（リダイレクション対応） */
static t_cmd *parse_simple_command(t_token_stream *tokens)
{
	t_cmd *cmd = create_cmd();
	if (!cmd)
		return NULL;

	int argc = 0;
	cmd->argv = malloc(sizeof(char *));
	if (!cmd->argv) {
		free(cmd);
		return NULL;
	}
	cmd->argv[0] = NULL;

	t_redirect_list *last_redirect_list = NULL;

	// TOKEN_WORD とリダイレクションを処理
	while (tokens->current &&
		   (tokens->current->type == TOKEN_WORD ||
			tokens->current->type == TOKEN_REDIRECT)) {

		if (tokens->current->type == TOKEN_WORD) {
			// argv配列を拡張
			cmd->argv = extend_argv(cmd->argv, argc);
			if (!cmd->argv) {
				free_cmd(cmd);
				return NULL;
			}

			// 単語を追加
			cmd->argv[argc] = ft_strdup(tokens->current->value.word);
			if (!cmd->argv[argc]) {
				free_cmd(cmd);
				return NULL;
			}
			argc++;

			// 次のトークンへ
			tokens->current = tokens->current->next;
		} else if (tokens->current->type == TOKEN_REDIRECT) {
			// リダイレクションをパース
			t_redirect_list *new_redirect = parse_redirection(tokens);
			if (!new_redirect) {
				// parse_redirectionがNULLを返した場合、構文エラー
				free_cmd(cmd);
				return NULL;
			}

			// リダイレクションをリストに追加
			if (!cmd->redirects) {
				cmd->redirects = new_redirect;
				last_redirect_list = new_redirect;
			} else {
				last_redirect_list->next = new_redirect;
				last_redirect_list = new_redirect;
			}
		}
	}

	// コマンドが空の場合はエラー
	if (argc == 0) {
		free_cmd(cmd);
		return NULL;
	}

	return cmd;
}

/* パイプラインのパース（複数のコマンドをパイプで接続） */
static t_cmd *parse_pipeline(t_token_stream *tokens, t_parse_result *result)
{
	t_cmd *first_cmd = NULL;
	t_cmd *current_cmd = NULL;
	t_cmd *new_cmd = NULL;

	// 最初のコマンドをパース
	new_cmd = parse_simple_command(tokens);
	if (!new_cmd) {
		// パイプの前にコマンドがない場合のエラー
		if (tokens->current && tokens->current->type == TOKEN_PIPE) {
			result->error_msg = ft_strdup("syntax error near unexpected token '|'");
			result->error_line = tokens->current->line_number;
			result->error_column = tokens->current->column;
		} else if (tokens->current && tokens->current->type == TOKEN_REDIRECT) {
			// リダイレクションのエラー（ファイル名なし）
			result->error_msg = ft_strdup("syntax error near unexpected token 'newline'");
			result->error_line = tokens->current->line_number;
			result->error_column = tokens->current->column;
		}
		return NULL;
	}

	first_cmd = new_cmd;
	current_cmd = new_cmd;

	// パイプが続く限り処理
	while (tokens->current && tokens->current->type == TOKEN_PIPE) {
		// パイプトークンを消費
		tokens->current = tokens->current->next;

		// 次のコマンドをパース
		new_cmd = parse_simple_command(tokens);
		if (!new_cmd) {
			// パイプの後にコマンドがない場合のエラー
			result->error_msg = ft_strdup("syntax error: unexpected end after pipe");
			if (tokens->current) {
				result->error_line = tokens->current->line_number;
				result->error_column = tokens->current->column;
			}
			free_cmd(first_cmd);
			return NULL;
		}

		// コマンドをチェーンに追加
		current_cmd->next = new_cmd;
		current_cmd = new_cmd;
	}

	return first_cmd;
}

/* 演算子のタイプを判定 */
static t_connector get_connector_type(const char *op)
{
	if (!op)
		return CONN_NONE;

	if (ft_strncmp(op, "&&", 2) == 0)
		return CONN_AND;
	else if (ft_strncmp(op, "||", 2) == 0)
		return CONN_OR;
	else if (ft_strncmp(op, ";", 1) == 0)
		return CONN_SEMICOLON;

	return CONN_NONE;
}

/* メインのパース関数 - 複数のパイプラインを演算子で接続 */
t_parse_result *parse(t_token_stream *tokens)
{
	if (!tokens || !tokens->head)
		return NULL;

	// パース結果を作成
	t_parse_result *result = create_parse_result();
	if (!result)
		return NULL;

	// トークンストリームをリセット
	tokens->current = tokens->head;

	t_pipeline *first_pipeline = NULL;
	t_pipeline *current_pipeline = NULL;

	// パイプラインのリストをパース
	while (tokens->current && tokens->current->type != TOKEN_EOF) {
		// 新しいパイプラインを作成
		t_pipeline *new_pipeline = create_pipeline();
		if (!new_pipeline) {
			free_pipeline(first_pipeline);
			free_parse_result(result);
			return NULL;
		}

		// パイプラインをパース
		t_cmd *cmds = parse_pipeline(tokens, result);
		if (!cmds) {
			free_pipeline(new_pipeline);
			free_pipeline(first_pipeline);
			if (!result->error_msg) {
				free_parse_result(result);
				return NULL;
			}
			// エラーメッセージがある場合
			result->ast = NULL;
			return result;
		}

		new_pipeline->cmds = cmds;

		// パイプラインをリストに追加
		if (!first_pipeline) {
			first_pipeline = new_pipeline;
			current_pipeline = new_pipeline;
		} else {
			current_pipeline->next = new_pipeline;
			current_pipeline = new_pipeline;
		}

		// 演算子をチェック
		if (tokens->current && tokens->current->type == TOKEN_OPERATOR) {
			current_pipeline->connector = get_connector_type(tokens->current->value.word);
			tokens->current = tokens->current->next;

			// 演算子の後にコマンドがない場合のエラー
			if (!tokens->current || tokens->current->type == TOKEN_EOF) {
				result->error_msg = ft_strdup("syntax error: unexpected end after operator");
				free_pipeline(first_pipeline);
				result->ast = NULL;
				return result;
			}
		}
	}

	result->ast = first_pipeline;
	return result;
}

void free_parse_result(t_parse_result *result)
{
	if (!result)
		return;

	free_pipeline(result->ast);
	free(result->error_msg);
	free(result);
}

void free_pipeline(t_pipeline *pipeline)
{
	while (pipeline) {
		t_pipeline *next = pipeline->next;
		free_cmd(pipeline->cmds);
		free(pipeline);
		pipeline = next;
	}
}

void free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return;

	// argv の解放
	if (cmd->argv) {
		for (int i = 0; cmd->argv[i]; i++) {
			free(cmd->argv[i]);
		}
		free(cmd->argv);
	}

	// リダイレクトの解放
	free_redirect(cmd->redirects);

	// 次のコマンドの解放（パイプチェーン）
	free_cmd(cmd->next);

	free(cmd);
}

void free_redirect(t_redirect_list *redirect)
{
	while (redirect) {
		t_redirect_list *next = redirect->next;
		free(redirect->file);
		free(redirect);
		redirect = next;
	}
}
