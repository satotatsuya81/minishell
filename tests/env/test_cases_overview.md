# Environment Tests ケース一覧表

**最終更新:** 2025/06/17  
**対象:** tests/env 配下の環境変数管理テスト  
**テストフレームワーク:** Criterion  
**パターン:** TDT (Table-Driven Tests)

---

## 📋 現在実装済みテストファイル

### 1. テストファイル構成

| ファイル名 | 種別 | 行数 | 対象機能 | 実行状況 | フレームワーク |
|-----------|------|------|----------|----------|---------------|
| `test_env_helpers.h` | ヘッダー | 15 | ヘルパー定義 | ✅ 正常 | Criterion |
| `test_env_helpers.c` | ユーティリティ | 24 | 比較・表示関数 | ✅ 正常 | Criterion |
| `test_env_variable.c` | メインテスト | 200+ | 環境変数管理 | ❌ 未実行 | Criterion |

**総テストケース数:** 15+ ケース（推定）  
**実行可能性:** ❌ Criterion フレームワーク未セットアップ

---

## 🔍 詳細テストケース分析

### **test_env_variable.c** - 環境変数管理テスト

#### **Criterionテストスイート構成**

| テストスイート | テスト関数数 | 対象機能 | テストケース例 |
|---------------|-------------|----------|---------------|
| `env_basic` | 5+ | 基本操作 | 追加、取得、削除、解放 |
| `env_creation` | 3+ | 環境作成 | envp配列からの作成 |
| `env_edge_cases` | 4+ | エッジケース | NULL値、空文字、無効エントリ |
| `env_memory` | 3+ | メモリ管理 | リーク防止、適切な解放 |

#### **想定テストケース詳細**

##### **基本操作テスト（env_basic）**

| ケース番号 | テスト関数 | 入力 | 期待結果 | 検証項目 |
|-----------|----------|------|----------|----------|
| 1 | `test_env_add` | `"VAR=value"` | 追加成功 | 変数存在確認 |
| 2 | `test_env_get` | `"VAR"` | `"value"` | 値取得確認 |
| 3 | `test_env_remove` | `"VAR"` | 削除成功 | 変数不存在確認 |
| 4 | `test_env_update` | `"VAR=newvalue"` | 更新成功 | 新値確認 |
| 5 | `test_env_free` | 環境変数リスト | 解放成功 | メモリ解放確認 |

##### **環境作成テスト（env_creation）**

| ケース番号 | テスト関数 | 入力 | 期待結果 | 検証項目 |
|-----------|----------|------|----------|----------|
| 1 | `test_create_from_envp` | `char **envp` | 環境作成 | 全変数コピー |
| 2 | `test_create_empty` | `NULL` | 空環境作成 | 空リスト |
| 3 | `test_create_with_invalid` | 無効エントリ含む | 部分作成 | 有効エントリのみ |

##### **エッジケーステスト（env_edge_cases）**

| ケース番号 | テスト関数 | 入力 | 期待結果 | 検証項目 |
|-----------|----------|------|----------|----------|
| 1 | `test_null_key` | `NULL` キー | エラー処理 | NULL ハンドリング |
| 2 | `test_empty_key` | `""` キー | エラー処理 | 空文字処理 |
| 3 | `test_null_value` | `NULL` 値 | 適切処理 | NULL 値処理 |
| 4 | `test_malformed_entry` | `"INVALID_FORMAT"` | エラー処理 | フォーマット検証 |

##### **メモリ管理テスト（env_memory）**

| ケース番号 | テスト関数 | 対象操作 | 期待結果 | 検証項目 |
|-----------|----------|----------|----------|----------|
| 1 | `test_memory_leaks` | 大量追加削除 | リークなし | Valgrind確認 |
| 2 | `test_cleanup_on_error` | エラー時クリーンアップ | 適切解放 | エラー時メモリ |
| 3 | `test_boundary_memory` | 境界値メモリ | 正常動作 | 境界条件 |

---

## 🏗️ **Criterionフレームワーク詳細**

### **テストアーキテクチャ**

#### **1. テストスイート定義**
```c
// Criterionテストスイート構造
Test(env_basic, test_env_add) {
    // テスト実装
    cr_assert_not_null(result);
    cr_assert_str_eq(env_get(env, "VAR"), "value");
}
```

#### **2. ヘルパー関数パターン**
```c
// test_env_helpers.c より
typedef struct {
    char *key;
    char *value;
} expected_env_entry;

void assert_env_equals(t_env_var *env, 
                      expected_env_entry *expected, 
                      int expected_count);
```

#### **3. 期待データ構造**
```c
// 期待値配列パターン
expected_env_entry expected_vars[] = {
    {"PATH", "/usr/bin:/bin"},
    {"HOME", "/home/user"},
    {"USER", "testuser"}
};
```

### **Criterionの優位点**

| 特徴 | 説明 | 利点 |
|------|------|------|
| **自動テスト発見** | `Test()` マクロで自動認識 | テスト追加が簡単 |
| **詳細アサーション** | `cr_assert_*` 豊富なマクロ | 明確なエラーメッセージ |
| **テスト隔離** | 各テスト独立実行 | 副作用なし |
| **レポート機能** | 詳細なテスト結果 | デバッグ支援 |
| **メモリ検証** | 組み込みメモリチェック | リーク検出 |

---

## 🚨 **現在の問題と制約**

### **実行阻害要因**

#### **1. Criterionフレームワーク未セットアップ**

**問題詳細:**
```bash
# 現在のエラー（推測）
fatal error: criterion/criterion.h: No such file or directory
```

**解決手順:**
```bash
# Ubuntu/Debian
sudo apt-get install libcriterion-dev

# macOS  
brew install criterion

# 手動インストール
git clone https://github.com/Snaipe/Criterion
cd Criterion
mkdir build && cd build
cmake .. && make && sudo make install
```

#### **2. 依存関数不足**

**問題:**
- `env_get()` 関数が見つからない
- `env_create_from_envp()` 関数参照が不明確

**影響:**
- コンパイルエラーでテスト実行不可
- 環境変数操作の核心機能テスト不可

#### **3. ビルドシステム統合**

**問題:**
- Makefileに Criterion用ターゲットなし
- ライブラリリンク設定なし

**必要設定:**
```makefile
# Makefile追加必要項目
CRITERION_LIBS = -lcriterion
ENV_TEST_NAME = tests/env/test_env_variable
ENV_TEST_SRCS = tests/env/test_env_variable.c \
                tests/env/test_env_helpers.c \
                $(ENV_SRCS) $(UTILS_SRCS)

test_env: $(ENV_TEST_NAME)
    @./$(ENV_TEST_NAME)

$(ENV_TEST_NAME): $(ENV_TEST_SRCS)
    $(CC) $(CFLAGS) $(ENV_TEST_SRCS) $(CRITERION_LIBS) -o $@
```

---

## 🔍 **推定テストカバレッジ分析**

### **環境変数操作関数（推定）**

| 関数名 | テスト予定 | 重要度 | テストケース数 |
|--------|-----------|--------|---------------|
| `env_add()` | ✅ 計画済み | **高** | 3ケース |
| `env_get()` | ✅ 計画済み | **高** | 3ケース |
| `env_remove()` | ✅ 計画済み | **高** | 4ケース |
| `env_update()` | ✅ 計画済み | **高** | 2ケース |
| `env_create_from_envp()` | ✅ 計画済み | **中** | 3ケース |
| `env_free()` | ✅ 計画済み | **高** | 2ケース |
| `env_to_array()` | ❓ 不明 | **中** | 推定2ケース |

### **エラーケースカバレッジ（推定）**

| エラー種別 | カバレッジ | テストケース |
|-----------|-----------|-------------|
| **NULL パラメータ** | ✅ 想定 | 各関数で1ケース |
| **無効フォーマット** | ✅ 想定 | 2ケース |
| **メモリ不足** | ❓ 不明 | 要実装確認 |
| **重複キー処理** | ✅ 想定 | 1ケース |
| **存在しないキー** | ✅ 想定 | 2ケース |

---

## ✅ **セットアップ・実行計画**

### **Phase 1: 環境構築 (1-2日)**

#### **1. Criterionインストール**
```bash
# 推奨: パッケージマネージャー使用
sudo apt-get update
sudo apt-get install libcriterion-dev

# 確認
pkg-config --cflags --libs criterion
```

#### **2. Makefile統合**
```makefile
# tests/env用ターゲット追加
test_env: $(ENV_TEST_NAME)
    @echo "Running environment variable tests..."
    @./$(ENV_TEST_NAME)

$(ENV_TEST_NAME): $(LIBFT_A) $(ENV_TEST_SRCS)
    @mkdir -p tests/env
    $(CC) $(CFLAGS) $(INCLUDES) $(ENV_TEST_SRCS) \
          -L$(LIBFT_DIR) -lft -lcriterion -o $@
```

#### **3. 依存関数確認**
```bash
# 不足関数の所在確認
grep -r "env_get" src/
grep -r "env_create_from_envp" src/
```

### **Phase 2: テスト実行・修正 (3-5日)**

#### **1. コンパイルエラー修正**
- 不足関数の実装または参照修正
- インクルードパス調整
- リンクエラー解決

#### **2. テスト実行・デバッグ**
```bash
make test_env
# エラー解析・修正
# テスト追加・調整
```

#### **3. 結果検証**
- 全テスト合格確認
- メモリリークチェック
- カバレッジ測定

### **Phase 3: 拡張・統合 (2-3日)**

#### **1. 不足テスト追加**
- エッジケース拡張
- 性能テスト追加
- 統合テスト作成

#### **2. CI/CD統合**
- 自動テスト実行設定
- レポート生成
- 品質ゲート設定

---

## 📊 **期待される品質効果**

### **実行可能になった場合の予想結果**

| メトリクス | 現在 | Phase 1後 | Phase 3後 |
|-----------|------|----------|-----------|
| **実行可能性** | 0% | 90% | 100% |
| **カバレッジ** | 0% | 80% | 95% |
| **テストケース** | 0 | 15+ | 25+ |
| **成功率** | N/A | 85%+ | 95%+ |

### **品質向上効果**

#### **開発効率向上**
- **自動テスト:** 手動テスト不要
- **早期バグ発見:** 開発段階でのエラー検出
- **リファクタリング安全性:** 変更による影響把握

#### **コード品質向上**
- **メモリ管理:** リーク検出・防止
- **エラーハンドリング:** 境界条件の確認
- **API設計:** 使いやすいインターフェース検証

---

## 💡 **実装ガイドライン**

### **Criterionテストパターン**

```c
// 基本テストパターン
Test(env_basic, add_variable) {
    // Arrange
    t_env_var *env = env_create_empty();
    
    // Act
    int result = env_add(env, "TEST_VAR", "test_value");
    
    // Assert
    cr_assert_eq(result, 0);
    cr_assert_str_eq(env_get(env, "TEST_VAR"), "test_value");
    
    // Cleanup
    env_free(env);
}
```

### **ヘルパー関数活用**

```c
// test_env_helpers.c活用例
Test(env_creation, from_envp_array) {
    // 期待値定義
    expected_env_entry expected[] = {
        {"PATH", "/usr/bin"},
        {"HOME", "/home/test"}
    };
    
    // 環境作成
    char *envp[] = {"PATH=/usr/bin", "HOME=/home/test", NULL};
    t_env_var *env = env_create_from_envp(envp);
    
    // ヘルパー関数で検証
    assert_env_equals(env, expected, 2);
    
    env_free(env);
}
```

---

**このEnvironmentテストスイートは、Criterionフレームワークの専門的な機能を活用した高品質な単体テストとして、minishellの環境変数管理機能の信頼性確保に重要な役割を果たす予定である。**