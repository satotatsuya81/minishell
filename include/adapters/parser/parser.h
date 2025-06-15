#ifndef PARSER_H
#define PARSER_H

#include "domain/token.h"

/* コマンド構造体 */
typedef struct s_cmd {
	char **argv;                   // コマンドと引数の配列
	struct s_redirect_list *redirects;  // リダイレクションのリスト
	struct s_cmd *next;           // パイプでつながる次のコマンド
} t_cmd;

/* リダイレクション構造体 */
typedef struct s_redirect_list {
	t_redirect_type type;            // リダイレクションの種類
	char *file;                   // ファイル名
	int fd;                       // ファイルディスクリプタ（必要な場合）
	struct s_redirect_list *next;      // 次のリダイレクション
} t_redirect_list;

/* パイプライン構造体（コマンドの連鎖） */
typedef struct s_pipeline {
	t_cmd *cmds;                  // コマンドのリスト
	struct s_pipeline *next;      // 演算子でつながる次のパイプライン
	int connector;                // 接続演算子（AND, OR, SEMICOLON）
} t_pipeline;

/* 接続演算子の定義 */
typedef enum {
	CONN_NONE = 0,
	CONN_AND,      // &&
	CONN_OR,       // ||
	CONN_SEMICOLON // ;
} t_connector;

/* パース結果構造体 */
typedef struct s_parse_result {
	t_pipeline *ast;              // パース結果のAST
	char *error_msg;              // エラーメッセージ
	int error_line;               // エラー行
	int error_column;             // エラー列
} t_parse_result;

/* パーサー関数のプロトタイプ */
t_parse_result *parse(t_token_stream *tokens);
void free_parse_result(t_parse_result *result);
void free_pipeline(t_pipeline *pipeline);
void free_cmd(t_cmd *cmd);
void free_redirect(t_redirect_list *redirect);

#endif /* PARSER_H */
