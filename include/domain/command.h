#ifndef COMMAND_H
#define COMMAND_H

#include "../domain/token_types.h"

/* コマンド構造体 */
typedef struct s_cmd {
    char **argv;                       // コマンドと引数の配列
    struct s_cmd_redirect *redirects;  // リダイレクションのリスト
    struct s_cmd *next;               // パイプでつながる次のコマンド
} t_cmd;

/* リダイレクション構造体 */
typedef struct s_cmd_redirect {
    t_redirect_type type;         // リダイレクションの種類
    char *file;                   // ファイル名
    int fd;                       // ファイルディスクリプタ（必要な場合）
    struct s_cmd_redirect *next;  // 次のリダイレクション
} t_cmd_redirect;

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

/* コマンド関連のメモリ解放関数 */
void free_pipeline(t_pipeline *pipeline);
void free_cmd(t_cmd *cmd);
void free_redirect(t_cmd_redirect *redirect);

#endif /* COMMAND_H */