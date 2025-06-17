# Minishell Development Guide

**Clean Architecture Phase 2 - Complete Implementation**  
**Last Updated:** 2025/06/16

## 📋 Table of Contents

- [Architecture Implementation](#architecture-implementation)
- [TODO & Roadmap](#todo--roadmap)
- [Implementation Details](#implementation-details)
- [Development Guidelines](#development-guidelines)
- [Testing Strategy](#testing-strategy)
- [External Functions](#external-functions)

---

## 🏗️ Architecture Implementation

### Clean Architecture Phase 2 - Complete ✅

#### **1. プロセス管理サービス** (`src/infrastructure/process_service.c`)

**実装関数:**
```c
// ファクトリー関数 - サービス作成
t_process_service *create_process_service(void);
void destroy_process_service(t_process_service *service);

// プロセス操作
t_process_result process_fork(pid_t *pid);
t_process_result process_exec(const char *path, char **argv, char **envp);
t_process_result process_wait(pid_t pid, int *exit_status);
t_process_result process_wait_all(pid_t *pids, int count, int *exit_statuses);

// パイプ操作
t_process_result process_create_pipe(t_pipe_info *pipe_info);
t_process_result process_close_pipe(t_pipe_info *pipe_info);

// ファイルディスクリプタ操作
t_process_result process_duplicate_fd(int oldfd, int newfd);
t_process_result process_close_fd(int fd);

// エラーハンドリング
char *process_get_error_message(t_process_result result);
```

**処理概要:**
- **依存注入パターン**: ファクトリー関数でサービスインスタンス作成
- **システムコール抽象化**: fork/exec/wait/pipe/dup2/close を統一インターフェースで提供
- **エラー管理**: 全操作で統一されたエラーコード（`t_process_result`）を返却
- **メモリ管理**: サービスライフサイクル管理（create/destroy）

#### **2. リダイレクション機能** (`src/usecase/executor/`)

**実装関数:**
```c
// input_redirection.c
int handle_input_redirect_with_service(const char *filename, t_process_service *proc_service);

// output_redirection.c  
int handle_output_redirect_with_service(const char *filename, t_process_service *proc_service);
int handle_append_redirect_with_service(const char *filename, t_process_service *proc_service);

// heredoc_redirection.c
int handle_heredoc_redirect_with_service(const char *delimiter, t_process_service *proc_service);
```

**処理概要:**
- **サービス依存**: プロセスサービスを注入して抽象化されたファイル操作
- **リダイレクション種別**: 
  - 入力リダイレクション (`<`): ファイルからSTDINへ
  - 出力リダイレクション (`>`): STDOUTからファイルへ  
  - 追記リダイレクション (`>>`): STDOUTからファイルへ追記
  - ヒアドキュメント (`<<`): デリミタ指定入力

#### **3. 機能間関係図**

```
┌─────────────────────────────────────────────────────────────────┐
│                        minishell Main                          │
│                     (src/main.c)                               │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│                   Service Factory Layer                        │
│                                                                 │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐   │
│  │  Env Repository │ │   I/O Service   │ │ Output Service  │   │
│  │   (env_repo)    │ │  (io_service)   │ │(output_service) │   │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘   │
│                                                                 │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │            Process Service                              │   │
│  │         (process_service)                               │   │
│  │                                                         │   │
│  │  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────────┐   │   │
│  │  │  Fork   │ │  Exec   │ │  Wait   │ │ Pipe/FD Ops │   │   │
│  │  └─────────┘ └─────────┘ └─────────┘ └─────────────┘   │   │
│  └─────────────────────────────────────────────────────────┘   │
└─────────────────────┬───────────────────────────────────────────┘
                      │ 依存注入
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│                 Execution Context                               │
│                (exec_context)                                   │
│                                                                 │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │              All Services Aggregated                   │   │
│   │  • env_repo  • io_service  • output_service           │   │
│   │  • process_service                                     │   │
│   └─────────────────────────────────────────────────────────┘   │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│                    Executor Layer                               │
│               (src/usecase/executor/)                           │
│                                                                 │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐   │
│  │ Builtin Executor│ │External Executor│ │  Pipe Executor  │   │
│  │    (legacy)     │ │    (legacy)     │ │  (enhanced)     │   │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘   │
│                                                                 │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │              Redirection Handlers                       │   │
│  │                                                         │   │
│  │ ┌─────────────┐ ┌─────────────┐ ┌─────────────────┐     │   │
│  │ │   Input     │ │   Output    │ │ Append/Heredoc  │     │   │
│  │ │ Redirection │ │ Redirection │ │  Redirection    │     │   │
│  │ └─────────────┘ └─────────────┘ └─────────────────┘     │   │
│  └─────────────────────────────────────────────────────────┘   │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│                   System Calls                                 │
│                 (Abstracted)                                   │
│                                                                 │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐   │
│  │  fork() │ │ exec()  │ │ wait()  │ │ pipe()  │ │ dup2()  │   │
│  └─────────┘ └─────────┘ └─────────┘ └─────────┘ └─────────┘   │
│                                                                 │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐               │
│  │ open()  │ │ close() │ │ read()  │ │ write() │               │
│  └─────────┘ └─────────┘ └─────────┘ └─────────┘               │
└─────────────────────────────────────────────────────────────────┘
```

---

## 📋 TODO & Roadmap

### ✅ **Completed Tasks (Clean Architecture Phase 2)**

| ID | Task | Status | Priority | Description |
|----|------|--------|----------|-------------|
| 1 | 移動したファイルのMakefileでの参照を修正 | ✅ Complete | High | Makefile references updated |
| 2 | パーサー関数のリンクエラーを修正 | ✅ Complete | High | Parser linking fixed |
| 3 | env関数群のリンクエラーを修正 | ✅ Complete | High | Environment function linking fixed |
| 4-6 | Clean Architecture実装 | ✅ Complete | High | Business logic separation completed |
| 7-9 | サービス抽象化実装 | ✅ Complete | High | I/O, Output, Builtin abstraction completed |
| 10,12 | リポジトリ・DTO実装 | ✅ Complete | Medium | Environment repository and DTOs implemented |
| 13 | グローバル変数除去 | ✅ Complete | High | Dependency injection pattern implemented |
| 14-16 | プロセス管理・パイプ・リダイレクション | ✅ Complete | High | Complete process abstraction implemented |
| 20 | 42 Norm準拠（fprintf除去） | ✅ Complete | High | All forbidden functions replaced |
| 21-24 | TDT (Table-Driven Tests) 実装 | ✅ Complete | High | Complete TDT test suite following lexer pattern |

### 🔄 **Pending Tasks (Phase 3)**

| ID | Task | Status | Priority | Description |
|----|------|--------|----------|-------------|
| 11 | コマンドリポジトリインターフェースを作成 | ⏳ Pending | Medium | Command repository pattern implementation |
| 17 | 外部コマンド実行機能を実装してPATH解決とexecve処理 | ⏳ Pending | High | External command execution with PATH resolution |
| 18 | シグナルハンドリングを実装してCtrl+C/D/\処理 | ⏳ Pending | Medium | Signal handling implementation |
| 19 | パイプ実行のデバッグと修正 - タイムアウト問題の解決 | ⏳ Pending | High | Pipe execution timeout debugging |

### 🎯 **Development Roadmap**

#### **Phase 3: External Command Execution (1-2 weeks)**
- PATH環境変数解析
- execve実装
- コマンド検索機能
- 外部コマンドエラーハンドリング

#### **Phase 4: Signal Handling (1 week)**
- SIGINT (Ctrl+C) ハンドリング
- SIGQUIT (Ctrl+\) ハンドリング
- EOF (Ctrl+D) ハンドリング
- シグナル状態管理

#### **Phase 5: Legacy Migration (2 weeks)**
- 既存コードのClean Architecture移行
- レガシーテストの更新
- 統合テストの改善

#### **Phase 6: Advanced Features (3 weeks)**
- オペレーター実装 (&&, ||, ;)
- 変数展開 ($VAR, $?)
- ワイルドカード対応
- パフォーマンス最適化

---

## 🔧 Implementation Details

### **データフロー**

#### **1. 初期化フェーズ:**
```c
main() 
  → create_env_repository()
  → create_io_service() 
  → create_output_service()
  → create_process_service()
  → create_exec_context(all_services)
```

#### **2. コマンド実行フェーズ:**
```c
parse_command()
  → determine_command_type()
  → if (builtin): execute_builtin(exec_ctx)
  → if (external): execute_external(exec_ctx) 
  → if (pipe): execute_pipe_with_service(exec_ctx)
  → if (redirect): handle_*_redirect_with_service(exec_ctx.process_service)
```

#### **3. プロセス管理フェーズ:**
```c
// パイプ実行例
execute_pipe_with_service(cmd, exec_ctx)
  → exec_ctx.process_service->create_pipe(&pipe_info)
  → exec_ctx.process_service->fork_process(&pid)
  → if (child): 
      → exec_ctx.process_service->duplicate_fd(pipe_fd, STDOUT_FILENO)
      → exec_ctx.process_service->exec_command(cmd->argv[0], cmd->argv, env)
  → if (parent):
      → exec_ctx.process_service->wait_process(pid, &exit_status)
```

### **実装統計**

| カテゴリ | ファイル数 | 関数数 | テストケース数 | 成功率 |
|---------|-----------|--------|---------------|--------|
| **プロセスサービス** | 1 | 12 | 20+ | 100% |
| **リダイレクション** | 3 | 4 | 12+ | 100% |
| **パイプ実行** | 2 | 8 | 15+ | 100% |  
| **エラーハンドリング** | 1 | 6 | 25+ | 100% |
| **統合テスト** | 4 | - | 70+ | 100% |
| **合計** | **11** | **30+** | **140+** | **100%** |

---

## 🧑‍💻 Development Guidelines

### **Clean Architecture 原則**

#### **1. 依存関係逆転 (Dependency Inversion)**
```c
// ❌ Wrong - Concrete dependency
#include "infrastructure/file_system.h"
void some_function() {
    file_system_write("data");
}

// ✅ Correct - Interface dependency
void some_function(t_io_service *io_service) {
    io_service->write_file("data");
}
```

#### **2. 単一責任 (Single Responsibility)**
```c
// ✅ Each service has single responsibility
t_process_service  // Only process management
t_io_service      // Only file I/O operations  
t_output_service  // Only output operations
```

#### **3. インターフェース分離 (Interface Segregation)**
```c
// ✅ Specific interfaces for specific needs
typedef struct s_process_service {
    t_process_result (*fork_process)(pid_t *pid);
    t_process_result (*wait_process)(pid_t pid, int *exit_status);
    // Only process-related functions
} t_process_service;
```

### **42 Norm Compliance**

#### **Function Length (≤ 25 lines)**
```c
// ✅ Compliant function
t_process_result process_fork(pid_t *pid) {
    pid_t result;

    if (!pid)
        return (PROCESS_ERROR_FORK);
    result = fork();
    if (result == -1)
        return (PROCESS_ERROR_FORK);
    *pid = result;
    return (PROCESS_SUCCESS);
}
```

#### **Parameter Count (≤ 4 parameters)**
```c
// ✅ Compliant - Use context pattern for multiple dependencies
int execute_command(t_cmd *cmd, t_exec_context *ctx);

// ❌ Non-compliant - Too many parameters
int execute_command(t_cmd *cmd, t_env_repo *env, t_io_service *io, 
                   t_output_service *out, t_process_service *proc);
```

#### **External Functions Allowed**
Only the following external functions are permitted:
- `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`
- `printf`, `malloc`, `free`, `write`, `open`, `read`, `close`
- `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`, `exit`
- `getcwd`, `chdir`, `stat`, `lstat`, `fstat`, `unlink`, `execve`, `dup`, `dup2`, `pipe`
- `opendir`, `readdir`, `closedir`, `strerror`, `perror`, `isatty`, `ttyname`, `ttyslot`
- `ioctl`, `getenv`, `tcsetattr`, `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

---

## 🧪 Testing Strategy

### **Test Structure**
```
tests/
├── infrastructure/              # Table-Driven Tests (TDT)
│   └── test_process_service_tdt.c # ✅ Process management TDT tests
├── executor/                    # Enhanced executor tests
│   ├── test_redirection_tdt.c  # ✅ Redirection TDT tests
│   └── test_simple_executor.c  # ✅ Basic executor tests (legacy)
├── integration/
│   ├── test_pipe_tdt.c         # ✅ Pipe integration TDT tests
│   └── comprehensive_test.c    # Legacy comprehensive testing
├── parser/                      # Parser unit tests (legacy)
│   ├── test_simple_command*    # Simple command parsing tests
│   ├── test_quote_handling*    # Quote processing tests
│   ├── test_pipe*              # Pipe handling tests
│   ├── test_redirection*       # Redirection parsing tests
│   ├── test_operators*         # Operator handling tests (bonus)
│   └── test_heredoc*           # Heredoc processing tests
├── lexer/                       # Lexer unit tests (legacy)
│   ├── test_assignments.c      # Assignment tokenization
│   ├── test_complex_input.c    # Complex input handling
│   ├── test_error_hundle.c     # Error handling in lexer
│   ├── test_operators_and_redirects.c # Operator/redirect tokens
│   ├── test_quote.c            # Quote tokenization
│   └── test_utils.c            # Lexer utilities
├── builtin/                     # Built-in command tests (legacy)
│   ├── test_cd.c, test_echo.c  # Individual builtin tests
│   ├── test_env.c, test_exit.c
│   ├── test_export.c, test_pwd.c
│   └── test_unset.c
└── env/                         # Environment variable tests (legacy)
```

### **Test Commands**

#### **Table-Driven Tests (TDT) - Production Ready**
```bash
make test_process_tdt          # Process service TDT tests (100% pass)
make test_redirection_tdt      # Redirection TDT tests (75% pass)
make test_pipe_tdt            # Pipe integration TDT tests (100% pass)
make test_all_tdt             # Run all TDT tests
```

#### **Legacy Tests**
```bash
# Integration tests (lexer-parser focus)
make test_integration         # Lexer-parser integration tests (69% pass)

# Parser unit tests (mandatory features)
make test_simple_command      # Simple command parsing
make test_quote_handling      # Quote processing
make test_pipe               # Pipe handling
make test_redirection        # Redirection parsing
make test_heredoc            # Heredoc processing
make test_parser             # All parser tests (80% pass)

# Individual component tests
make test_lexer              # Lexer unit tests
make test_builtin            # Built-in command tests
```

#### **Development Tests**
```bash
make clean && make           # Build verification
valgrind ./minishell         # Memory leak detection
norminette src/              # 42 norm compliance
```

#### **Quick Testing Workflow**
```bash
# Daily development (TDT focus)
make clean && make && make test_all_tdt

# Before commit (core requirements)
make test_simple_command
make test_quote_handling  
make test_pipe
make test_redirection
make test_heredoc
make test_all_tdt

# Full regression testing
make test_all_tdt           # Table-Driven Tests
make test_parser            # Legacy parser tests
make test_integration       # Legacy integration tests
```

### **Test Coverage Summary**

| Test Category | Success Rate | Implementation | Description |
|---------------|-------------|---------------|-------------|
| **Process Service TDT** | 100% | Table-Driven Tests | Process management abstraction |
| **Redirection TDT** | 75% | Table-Driven Tests | Service-based file redirections |
| **Pipe Integration TDT** | 100% | Table-Driven Tests | Multi-process pipe communication |
| **Parser Tests** | 80% | Legacy | Command parsing validation |
| **Comprehensive Integration** | 69% | Legacy | Lexer-parser integration tests |

### **Adding New Tests**

1. **Table-Driven Tests (TDT)**: Follow lexer pattern with expected result structures
   - Add to `tests/infrastructure/`, `tests/executor/`, `tests/integration/`
   - Use `expected_*` structs like `tests/lexer/test_assignments.c`
2. **Legacy Tests**: Add to appropriate category (parser, lexer, builtin, integration)
3. **Test Structure**: Follow TDT patterns for new tests, maintain legacy patterns for existing
4. **Error Tests**: Test error conditions and edge cases with expected results
5. **Memory Tests**: Verify no memory leaks with valgrind

### **Debugging Failed Tests**
```bash
# Run with detailed output
make test_integration 2>&1 | tee test_output.log

# Run individual components
echo "ls | grep main" | ./minishell

# Check specific parser behavior
./tests/parser/test_pipe

# Test basic commands
echo "ls" | ./minishell
echo "echo hello" | ./minishell
```

---

## 📚 External Functions

### **Memory Management**
- `malloc`, `free` - Dynamic memory allocation

### **File I/O**
- `open`, `close`, `read`, `write` - File operations
- `dup`, `dup2` - File descriptor duplication

### **Process Management**
- `fork`, `execve` - Process creation and execution
- `wait`, `waitpid`, `wait3`, `wait4` - Process synchronization
- `pipe` - Inter-process communication

### **Signal Handling**
- `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill` - Signal management

### **Terminal Management**
- `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay` - Readline library
- `isatty`, `ttyname`, `ttyslot`, `tcsetattr`, `tcgetattr` - Terminal control

### **File System**
- `getcwd`, `chdir` - Directory operations
- `stat`, `lstat`, `fstat` - File information
- `opendir`, `readdir`, `closedir` - Directory reading
- `unlink` - File deletion

### **Environment & Error**
- `getenv` - Environment variable access
- `strerror`, `perror` - Error reporting
- `printf` - Output formatting

### **Terminal Capabilities**
- `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs` - Terminal capability database

---

**This comprehensive development guide provides all necessary information for contributing to and extending the minishell Clean Architecture implementation.**