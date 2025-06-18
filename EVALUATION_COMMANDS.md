# 42 Minishell 評価用コマンド集

## 📋 概要

この文書は42 Schoolのminishell評価において、bashとの挙動を比較するためのコマンド集です。
各コマンドは評価項目に対応しており、bashとminishellの両方で実行して挙動を確認できます。

---

## 🔧 基本的な使用方法

```bash
# bash での実行
bash$ command

# minishell での実行  
minishell> command
```

---

## 📊 評価項目別コマンド集

### 1. 基本シェル機能

#### 1.1 プロンプト表示とコマンド実行
```bash
# 基本コマンド実行
echo "Hello World"
pwd
ls
```

#### 1.2 実行可能ファイル検索 (PATH)
```bash
# PATH変数内のコマンド
ls -la
cat /etc/hostname
which ls
```

#### 1.3 相対・絶対パスでの実行
```bash
# 絶対パス
/bin/echo "absolute path"
/usr/bin/env

# 相対パス (存在する場合)
./minishell
../bin/ls
```

---

### 2. 組み込みコマンド

#### 2.1 echo (オプション付き)
```bash
echo "simple echo"
echo -n "no newline"
echo -n "test" && echo " continuation"
echo "multiple" "arguments" "test"
```

#### 2.2 cd (相対・絶対パスのみ)
```bash
cd /tmp
pwd
cd ..
pwd
cd /home
pwd
cd
pwd
```

#### 2.3 pwd (オプションなし)
```bash
pwd
cd /var/log
pwd
cd ~
pwd
```

#### 2.4 export (オプションなし)
```bash
export TEST_VAR=42
echo $TEST_VAR
export PATH="/custom/path:$PATH"
echo $PATH
export EMPTY_VAR=
echo $EMPTY_VAR
```

#### 2.5 unset (オプションなし)
```bash
export TEST_VAR=hello
echo $TEST_VAR
unset TEST_VAR
echo $TEST_VAR
```

#### 2.6 env (オプション・引数なし)
```bash
env
env | grep USER
env | head -5
```

#### 2.7 exit (オプションなし)
```bash
exit
exit 42
exit 0
```

---

### 3. クォート処理

#### 3.1 シングルクォート (メタ文字完全無効化)
```bash
echo '$USER'
echo '$(date)'
echo '*'
echo '|'
echo '>'
echo '<'
echo '"nested quotes"'
```

#### 3.2 ダブルクォート ($以外のメタ文字無効化)
```bash
echo "$USER"
echo "Current user: $USER"
echo "Path: $PATH"
echo "Literal symbols: | > < *"
echo "Quote in quote: 'test'"
```

#### 3.3 ネストされたクォート
```bash
echo 'single "double" single'
echo "double 'single' double"
echo '"mixed quotes test"'
echo "'mixed quotes test'"
```

---

### 4. 変数展開

#### 4.1 環境変数展開
```bash
echo $USER
echo $HOME
echo $PATH
echo $PWD
echo $SHELL
```

#### 4.2 終了ステータス変数
```bash
echo $?
/bin/false
echo $?
/bin/true  
echo $?
nonexistentcommand
echo $?
```

#### 4.3 変数展開とクォート
```bash
echo $USER
echo "$USER"
echo '$USER'
echo "User is $USER"
echo 'User is $USER'
```

---

### 5. リダイレクション

#### 5.1 出力リダイレクション (>)
```bash
echo "test output" > test_file.txt
cat test_file.txt
ls > file_list.txt
cat file_list.txt
rm test_file.txt file_list.txt
```

#### 5.2 入力リダイレクション (<)
```bash
echo "test content" > input_test.txt
cat < input_test.txt
wc -l < input_test.txt
rm input_test.txt
```

#### 5.3 追記リダイレクション (>>)
```bash
echo "line 1" > append_test.txt
echo "line 2" >> append_test.txt
echo "line 3" >> append_test.txt
cat append_test.txt
rm append_test.txt
```

#### 5.4 Heredoc (<<)
```bash
cat << EOF
This is a heredoc test
Multiple lines
End of test
EOF

wc -l << DELIMITER
line one
line two
line three
DELIMITER
```

---

### 6. パイプ

#### 6.1 単純パイプ
```bash
echo "hello world" | cat
ls | wc -l
env | grep USER
echo "test" | cat -n
```

#### 6.2 複数パイプ
```bash
ls | head -5 | wc -l
echo "a b c d e" | tr ' ' '\n' | sort | uniq
env | grep PATH | cut -d= -f2 | tr ':' '\n'
```

#### 6.3 パイプとリダイレクション
```bash
ls | head -3 > pipe_output.txt
cat pipe_output.txt
echo "test" | cat > pipe_test.txt
cat pipe_test.txt
rm pipe_output.txt pipe_test.txt
```

---

### 7. シグナル処理

#### 7.1 Ctrl+C (SIGINT)
```bash
# Ctrl+Cを押してプロンプト表示確認
sleep 10
# Ctrl+C を押す
echo "After Ctrl+C"
```

#### 7.2 Ctrl+D (EOF)
```bash
# Ctrl+Dを押してシェル終了確認
cat
# Ctrl+D を押す
```

#### 7.3 Ctrl+\ (SIGQUIT)
```bash
# Ctrl+\を押して何も起こらないことを確認
sleep 5
# Ctrl+\ を押す
echo "After Ctrl+\\"
```

---

### 8. エラーハンドリング

#### 8.1 コマンド未発見
```bash
nonexistentcommand
echo $?
invalidcommand123
echo $?
```

#### 8.2 パーミッションエラー
```bash
# ファイルが実行権限なしの場合
echo "#!/bin/bash" > test_script
echo "echo test" >> test_script
./test_script
chmod +x test_script
./test_script
rm test_script
```

#### 8.3 ディレクトリ関連エラー
```bash
cd /nonexistent/directory
echo $?
cd /root 2>/dev/null || echo "Permission denied as expected"
echo $?
```

---

### 9. 複合コマンド例

#### 9.1 実際の使用例
```bash
export PATH="/bin:/usr/bin:$PATH"
echo "Current user: $USER" | cat > user_info.txt
cat user_info.txt
ls -la | grep "\.txt$" | wc -l
rm user_info.txt
```

#### 9.2 変数操作テスト
```bash
export TEST="initial value"
echo $TEST
export TEST="$TEST modified"
echo $TEST
unset TEST
echo $TEST
```

#### 9.3 パイプとリダイレクションの組み合わせ
```bash
ls -la | head -5 > temp_list.txt
cat temp_list.txt | wc -l
echo "Total files shown: " $(cat temp_list.txt | wc -l)
rm temp_list.txt
```

---

## 🔍 比較検証ポイント

### 正常動作確認
- [ ] コマンドの出力が同一
- [ ] 終了ステータス ($?) が同一
- [ ] ファイル作成/削除が正常
- [ ] 変数展開が正確

### エラー処理確認
- [ ] エラーメッセージの適切性
- [ ] 終了ステータスの正確性
- [ ] 異常終了しない

### シグナル処理確認
- [ ] Ctrl+C で新しいプロンプト表示
- [ ] Ctrl+D でシェル終了
- [ ] Ctrl+\ で何も起こらない

---

## 📝 テスト実行方法

1. **bash での基準実行**
   ```bash
   bash
   # 上記コマンドを順次実行
   ```

2. **minishell での比較実行**
   ```bash
   ./minishell
   # 同じコマンドを順次実行
   ```

3. **自動比較テスト**
   ```bash
   # 評価テストスイートの実行
   ./tests/evaluation/final_perfect_test.sh
   ```

---

## ⚠️ 注意事項

### 実装範囲外の機能
- セミコロン (`;`) による複数コマンド
- 論理演算子 (`&&`, `||`)
- バックスラッシュエスケープ (`\`)
- ワイルドカード (`*`, `?`, `[...]`)
- コマンド置換 (`$(command)`, `\`command\``)
- 算術展開 (`$((expression))`)

### 対象外オプション
- `cd` の `-L`, `-P` オプション
- `echo` の `-e`, `-E` オプション (`-n` のみ実装)
- 各コマンドのヘルプオプション (`--help`)

---

*このコマンド集は42 School minishellプロジェクトの評価基準に基づいて作成されています。*
*最終更新: 2025/06/18*