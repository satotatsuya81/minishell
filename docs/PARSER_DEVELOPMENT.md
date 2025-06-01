# パーサー開発ドキュメント

## 開発方針

### 基本方針
- **TDD（テスト駆動開発）**: テストを先に書いてから実装
- **段階的実装**: 簡単な機能から順に実装
- **既存構造の尊重**: チーム開発のため、既存の構造体（Token, TokenStream）は変更しない
- **エラーハンドリング**: 各段階でエラー処理を適切に実装

### 開発フェーズ
1. **基本機能**: 単純なコマンド解析
2. **標準機能**: パイプ、リダイレクション、演算子
3. **複合機能**: 複数機能の組み合わせ
4. **エラー処理**: 文法エラーの検出と報告

## 実装状況

### ✅ 完了したタスク
- [x] 単純なコマンドのパース（例: `ls`, `pwd`）
- [x] 引数付きコマンドのパース（例: `ls -la`, `echo hello`）
- [x] 複数引数のコマンドのパース（例: `echo hello world`）
- [x] クォート処理（レクサーが処理するため、パーサーでは追加実装不要）
- [x] libftへの移行（strdup → ft_strdup）
- [x] 単一パイプ（`cmd1 | cmd2`）
- [x] 複数パイプ（`cmd1 | cmd2 | cmd3`）
- [x] パイプのエラーハンドリング（`|` の前後にコマンドがない場合）
- [x] 入力リダイレクション（`<`）
- [x] 出力リダイレクション（`>`）
- [x] 追記リダイレクション（`>>`）
- [x] 複数リダイレクション
- [x] パイプとリダイレクションの組み合わせ
- [x] AND演算子（`&&`）
- [x] OR演算子（`||`）
- [x] セミコロン（`;`）
- [x] 複数の演算子の組み合わせ
- [x] 演算子とパイプの組み合わせ

### 🚧 実装中
- [ ] ヒアドキュメント（`<<`）

### 📋 実装予定（優先順位順）

#### フェーズ1: 基本機能
- [x] 単純なコマンドのパース（例: `ls`, `pwd`）
- [x] 引数付きコマンドのパース（例: `ls -la`, `echo hello`）
- [x] クォート処理（シングル・ダブルクォート） ※レクサーの責務
- [ ] 環境変数の認識（$VARの形式）

#### フェーズ2: リダイレクション
- [x] 入力リダイレクション（`<`）
- [x] 出力リダイレクション（`>`）
- [x] 追記リダイレクション（`>>`）
- [ ] ヒアドキュメント（`<<`）
- [x] 複数リダイレクションの処理

#### フェーズ3: パイプ
- [x] 単一パイプ（`cmd1 | cmd2`）
- [x] 複数パイプ（`cmd1 | cmd2 | cmd3`）
- [x] パイプとリダイレクションの組み合わせ

#### フェーズ4: 演算子
- [ ] AND演算子（`&&`）
- [ ] OR演算子（`||`）
- [ ] セミコロン（`;`）
- [ ] 演算子の優先順位処理

#### フェーズ5: エラーハンドリング
- [ ] 文法エラーの検出
- [ ] エラーメッセージの生成
- [ ] エラー位置の特定（行番号・列番号）

## テスト実装記録

### 実装済みテスト
```
test_parse_simple_command_ls() - "ls"をパースできることを確認
test_parse_command_with_args() - "ls -la"をパースできることを確認
test_parse_command_multiple_args() - "echo hello world"をパースできることを確認
test_single_quotes() - シングルクォートの処理を確認
test_double_quotes() - ダブルクォートの処理を確認
test_empty_quotes() - 空のクォートの処理を確認
test_special_chars_in_quotes() - クォート内の特殊文字を確認
test_simple_pipe() - 単一パイプのテスト
test_multiple_pipes() - 複数パイプのテスト
test_pipe_without_command() - パイプ前にコマンドがないエラーケース
test_pipe_at_end() - パイプ後にコマンドがないエラーケース
test_output_redirection() - 出力リダイレクションのテスト
test_input_redirection() - 入力リダイレクションのテスト
test_append_redirection() - 追記リダイレクションのテスト
test_multiple_redirections() - 複数リダイレクションのテスト
test_redirection_with_pipe() - パイプとリダイレクションの組み合わせ
test_and_operator() - AND演算子のテスト
test_or_operator() - OR演算子のテスト
test_semicolon_operator() - セミコロン演算子のテスト
test_mixed_operators() - 複数の演算子の組み合わせテスト
test_operators_with_pipes() - 演算子とパイプの組み合わせテスト
test_operator_without_command() - 演算子後にコマンドがないエラーテスト
```

### 次に実装するテスト
```
test_heredoc() - ヒアドキュメントのテスト
```

## 設計メモ

### AST構造（案）
```c
// コマンド構造体
typedef struct s_cmd {
    char **argv;                   // コマンドと引数の配列
    struct s_redirect *redirects;  // リダイレクションのリスト
    struct s_cmd *next;           // パイプでつながる次のコマンド
} t_cmd;

// リダイレクション構造体
typedef struct s_redirect {
    RedirectType type;            // リダイレクションの種類
    char *file;                   // ファイル名
    int fd;                       // ファイルディスクリプタ（必要な場合）
    struct s_redirect *next;      // 次のリダイレクション
} t_redirect;

// パイプライン構造体（コマンドの連鎖）
typedef struct s_pipeline {
    t_cmd *cmds;                  // コマンドのリスト
    struct s_pipeline *next;      // 演算子でつながる次のパイプライン
    int connector;                // 接続演算子（AND, OR, SEMICOLON）
} t_pipeline;
```

### パーサーインターフェース（案）
```c
// メインのパース関数
t_pipeline *parse(TokenStream *tokens);

// エラー情報を含むパース結果
typedef struct s_parse_result {
    t_pipeline *ast;              // パース結果のAST
    char *error_msg;              // エラーメッセージ
    int error_line;               // エラー行
    int error_column;             // エラー列
} t_parse_result;
```

## 課題・検討事項
- エラーテストの修正（リダイレクション後にファイル名がない場合のエラー結果返却）
- ヒアドキュメントの実装
- セミコロン区切りの実装

## 実装済み機能の詳細

### parse_simple_command()
- TOKEN_WORDトークンを収集してコマンドと引数に変換
- argv配列を動的に拡張
- リダイレクショントークンを検出して処理
- 空のコマンドはエラーとして処理

### parse_redirection()
- TOKEN_REDIRECTを受け取り、リダイレクション構造体を作成
- リダイレクションの後にファイル名が必要
- 複数のリダイレクションをリンクリストで管理

### parse_pipeline()
- 複数のコマンドをパイプで接続
- TOKEN_PIPEを検出してコマンドをチェーンに追加
- パイプの前後にコマンドがない場合のエラー処理

### parse()
- TokenStreamを受け取り、ASTを生成
- 複数のパイプラインを演算子で接続
- TOKEN_OPERATORを検出して適切なコネクタを設定
- エラー処理とメモリ管理を適切に実装

### get_connector_type()
- 演算子文字列からコネクタタイプを判定
- &&, ||, ; を識別して適切なt_connectorを返す

## 更新履歴
- 2025/01/06: 初版作成
- 2025/01/06: 単純なコマンドのパース機能を実装
- 2025/01/06: libftへの移行、クォート処理のテスト追加
- 2025/01/06: パイプ処理の実装（単一・複数パイプ、エラーハンドリング）
- 2025/01/06: リダイレクション処理の実装（入出力、追記、複数リダイレクション）
- 2025/01/06: AND/OR/セミコロン演算子の実装