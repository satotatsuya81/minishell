# Test Execution Guide - Mandatory Requirements Only

Quick reference for running tests that validate mandatory minishell requirements.

## 🏃‍♂️ Essential Commands

```bash
# Recommended: Test mandatory requirements
make test_integration_simple

# All parser unit tests
make test_parser

# Clean test artifacts
make testclean
```

## ⚠️ Note on Test Scope

This guide focuses on **mandatory requirements only** as specified in `doc/subject.txt`. Tests for bonus features (&&, ||, wildcards) are excluded as they are not required for the core functionality.

## 📋 Mandatory Test Categories

### 1. Integration Tests (Lexer + Parser) - Mandatory Features Only
**Location**: `tests/integration/`

| Command | Description | Test Count | Duration |
|---------|-------------|------------|----------|
| `make test_integration_simple` | Mandatory features only | 7 | <1s |

**Expected Output:**
```
=== Simple Lexer-Parser Integration Test ===
Testing: 'ls' -> PASSED
Testing: 'echo hello' -> PASSED
Testing: 'pwd' -> PASSED
Testing: 'cat file.txt' -> PASSED
Testing: 'echo 'hello world'' -> PASSED
Testing: 'echo "hello world"' -> PASSED
Testing: '   ls   ' -> PASSED

=== Summary ===
Passed: 7/7
✅ All basic tests passed!
```

### 2. Parser Unit Tests - Mandatory Features
**Location**: `tests/parser/`

| Command | Description | Mandatory |
|---------|-------------|-----------|
| `make test_simple_command` | Simple command parsing | ✅ Required |
| `make test_quote_handling` | Quote processing | ✅ Required |
| `make test_pipe` | Pipe handling | ✅ Required |
| `make test_redirection` | Redirection parsing | ✅ Required |
| `make test_heredoc` | Heredoc processing | ✅ Required |
| ~~`make test_operators`~~ | ~~Operator handling~~ | ❌ Bonus only |

**Expected Output:**
```
=== Parser Tests: Simple Commands ===
Test: parse simple command 'ls'... PASSED
Test: parse command with arguments 'ls -la'... PASSED
Test: parse command with multiple arguments 'echo hello world'... PASSED

All tests passed!
```

### 3. Lexer Unit Tests
**Location**: `tests/lexer/`
**Note**: These test mandatory tokenization features only.

## ⚡ Essential Test Commands

### Daily Development (Mandatory Features Only)
```bash
# Quick check after making changes
make clean && make && make test_integration_simple
```

### Before Commit (Mandatory Requirements)
```bash
# Test mandatory parser features
make test_simple_command
make test_quote_handling  
make test_pipe
make test_redirection
make test_heredoc

# Test integration
make test_integration_simple
```

### Debugging Specific Issues (Mandatory Features)
```bash
# Test basic commands
echo "ls" | ./minishell
echo "echo hello" | ./minishell

# Test pipes (mandatory)
echo "ls | grep main" | ./minishell
./tests/parser/test_pipe

# Test redirections (mandatory)
echo "echo hello > file.txt" | ./minishell
./tests/parser/test_redirection

# Test quotes (mandatory)
echo "echo 'hello world'" | ./minishell
./tests/parser/test_quote_handling
```

## 📊 Understanding Test Results (Mandatory Only)

### Integration Test Results

#### ✅ Success (Example)
```
Testing: Simple command -> PASSED
Testing: Command with argument -> PASSED
Testing: Pipe handling -> PASSED
Testing: Redirection -> PASSED
```

#### Summary (Mandatory Features)
```
=== Summary ===
Passed: 7/7
✅ All basic tests passed!
```

### Parser Test Results

#### ✅ Success
```
Test: parse simple command 'ls'... PASSED
All tests passed!
```

#### ❌ Failure
```
Test: parse simple command 'ls'... FAILED
Assertion failed: cmd->argv[0] != NULL
```

## 🐛 Common Issues & Solutions

### Compilation Errors
```bash
# If tests fail to compile
make clean
make
make test_integration_simple
```

### Missing Dependencies
```bash
# Ensure libft is built
make -C src/utils/libft bonus
```

## 🔍 Manual Testing (Mandatory Features)

### Basic Commands (Required)
```bash
# Build minishell
make

# Test mandatory functionality
echo "ls" | ./minishell
echo "echo hello" | ./minishell  
echo "pwd" | ./minishell
echo "env" | ./minishell
```

### Pipes (Required)
```bash
echo "ls | grep main" | ./minishell
echo "cat file | head" | ./minishell
```

### Redirections (Required)  
```bash
echo "echo test > file.txt" | ./minishell
echo "cat < file.txt" | ./minishell
echo "cat << EOF" | ./minishell
```

### Quotes (Required)
```bash
echo "echo 'hello world'" | ./minishell
echo "echo \"hello world\"" | ./minishell
```

### Environment Variables (Required)
```bash
echo "echo \$HOME" | ./minishell
echo "echo \$?" | ./minishell
```

## 📈 Mandatory Features Coverage

### ✅ Working (Required)
- Basic commands: `ls`, `echo hello`, `pwd`, `cd`, `env`, `export`, `unset`, `exit`
- Pipes: `ls | grep main`, `cat file | head | tail`
- Redirections: `echo > file`, `cat < file`, `cat << EOF`, `echo >> file`
- Quotes: `echo 'hello'`, `echo "world"`
- Variables: `echo $HOME`, `echo $?`

### 🔧 Areas to Test
- Built-in commands with options (e.g., `echo -n`)
- Error handling for invalid syntax
- Complex pipe chains
- Nested quotes and variable expansion

## 📞 Getting Help

- Check `BUG_REPORT.md` for known issues  
- Check `README.md` for detailed documentation
- Focus on mandatory requirements only