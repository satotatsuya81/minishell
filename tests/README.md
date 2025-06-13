# Minishell Test Suite

This directory contains comprehensive tests for the minishell project, covering lexer, parser, and integration testing.

## 📁 Directory Structure

```
tests/
├── README.md                    # This file
├── builtin/                     # Built-in command tests
│   ├── test_builtin.h
│   ├── test_cd.c
│   ├── test_echo.c
│   ├── test_env.c
│   ├── test_exit.c
│   ├── test_export.c
│   ├── test_pwd.c
│   └── test_unset.c
├── env/                         # Environment variable tests
├── lexer/                       # Lexer unit tests
│   ├── tester_lexer.h
│   ├── test_assignments.c
│   ├── test_complex_input.c
│   ├── test_error_hundle.c
│   ├── test_operators_and_redirects.c
│   ├── test_quote.c
│   └── test_utils.c
├── parser/                      # Parser unit tests
│   ├── test_simple_command*
│   ├── test_quote_handling*
│   ├── test_pipe*
│   ├── test_redirection*
│   ├── test_operators*
│   └── test_heredoc*
└── integration/                 # Integration tests (NEW)
    ├── integration_test.h
    ├── simple_integration_test.c
    ├── comprehensive_test.c
    └── lexer_parser_integration_test.c
```

## 🧪 Test Categories

### 1. Unit Tests
- **Lexer Tests**: Test tokenization of input strings
- **Parser Tests**: Test AST generation from tokens
- **Builtin Tests**: Test individual builtin commands

### 2. Integration Tests (NEW)
- **Simple Integration**: Basic lexer-parser workflow tests
- **Comprehensive Integration**: Full feature coverage with bug detection
- **Full Integration**: Complete test suite with detailed reporting

## 🚀 Running Tests (Mandatory Requirements Focus)

### Quick Start
```bash
# Recommended: Test mandatory requirements only
make test_integration_simple

# Run mandatory parser tests
make test_simple_command
make test_quote_handling  
make test_pipe
make test_redirection
make test_heredoc

# Clean test artifacts
make testclean
```

### ⚠️ Note
This test suite focuses on **mandatory requirements only** as specified in `doc/subject.txt`. Bonus features (&&, ||, wildcards) are not tested here.

### Individual Test Components

#### Integration Tests (Mandatory Only)
```bash
# Simple test (7 mandatory test cases)
make test_integration_simple
./tests/integration/simple_integration_test
```

#### Parser Tests (Mandatory Features)
```bash
# Individual mandatory parser tests
make test_simple_command && ./tests/parser/test_simple_command
make test_quote_handling && ./tests/parser/test_quote_handling
make test_pipe && ./tests/parser/test_pipe
make test_redirection && ./tests/parser/test_redirection
make test_heredoc && ./tests/parser/test_heredoc

# Note: test_operators is excluded (bonus feature only)
```

## 📝 Mandatory Test Cases Overview

### Integration Test Cases (simple_integration_test.c)

#### ✅ Basic Commands (Required)
| Test Case | Description | Status |
|-----------|-------------|---------|
| `ls` | Simple command | ✅ PASS |
| `echo hello` | Command with argument | ✅ PASS |
| `pwd` | Simple builtin | ✅ PASS |
| `cat file.txt` | Command with file | ✅ PASS |

#### ✅ Quotes (Required)
| Test Case | Description | Status |
|-----------|-------------|---------|
| `echo 'hello world'` | Single quotes | ✅ PASS |
| `echo "hello world"` | Double quotes | ✅ PASS |

#### ✅ Whitespace Handling (Required)
| Test Case | Description | Status |
|-----------|-------------|---------|
| `   ls   ` | Command with spaces | ✅ PASS |

### Mandatory Features Coverage

#### ✅ Working (Required by Subject)
- **Basic commands**: `ls`, `echo hello`, `pwd`
- **Built-in commands**: `cd`, `pwd`, `env`, `export`, `unset`, `exit`  
- **Pipes**: `ls | grep main` (tested in parser tests)
- **Redirections**: `>`, `<`, `>>`, `<<` (tested in parser tests)
- **Single quotes**: `echo 'hello world'`
- **Double quotes**: `echo "hello world"`
- **Environment variables**: `$HOME`, `$?` (tested in parser tests)
- **Whitespace handling**: Proper tokenization

#### 🔧 Not Covered (Signal Handling - Out of Scope)
- Interactive mode (Ctrl-C, Ctrl-D, Ctrl-\\)
- Command execution (this is lexer/parser testing only)

#### ❌ Excluded (Bonus Features)
- AND/OR operators: `&&`, `||`
- Wildcards: `*`  
- Parentheses for priorities

## 📊 Test Results Summary (Mandatory Only)

### Current Status (2025/01/06)
- **Mandatory Test Cases**: 7 (simple integration)
- **Passed**: 7/7 (100%)
- **Parser Tests**: 5 mandatory tests available
- **Critical Issues**: Minimal (see BUG_REPORT.md)

### Success Rates
- **Basic Integration**: 100% (7/7 simple tests)
- **Core Lexer-Parser Flow**: ✅ Working
- **Mandatory Features**: ✅ Properly implemented

## 🔧 Test Development

### Adding New Tests

#### Integration Tests
1. Edit `tests/integration/comprehensive_test.c`
2. Add test case to `test_case tests[]` array:
```c
{"your_command", "Description", should_succeed, "expected_cmd", argc, pipe_count}
```
3. Rebuild: `make test_integration`

#### Parser Tests
1. Create new test file in `tests/parser/`
2. Follow pattern from existing tests
3. Add Makefile rule
4. Update `test_parser` target

### Test Framework Features
- **Automatic cleanup**: Tests free all allocated memory
- **Detailed reporting**: Shows exact failure reasons
- **Bug categorization**: Separates mandatory vs bonus features
- **Regression testing**: Can verify fixes don't break existing functionality

## 📖 Usage Examples

### Development Workflow
```bash
# 1. Make changes to lexer/parser
vim src/usecase/lexer/lexer.c

# 2. Test changes
make clean && make
make test_integration

# 3. Check for regressions
make test_parser

# 4. View detailed results
cat BUG_REPORT.md
```

### Debugging Failed Tests
```bash
# Run with detailed output
make test_integration 2>&1 | tee test_output.log

# Run individual components
echo "ls | grep main" | ./minishell

# Check specific parser behavior
./tests/parser/test_pipe
```

## 🎯 Test Coverage

### Covered Features
- ✅ Basic command parsing
- ✅ Argument handling
- ✅ Pipe chains (single and multiple)
- ✅ All redirection types (<, >, >>, <<)
- ✅ Quote handling (single and double)
- ✅ Environment variable expansion
- ✅ Error detection and reporting
- ✅ Whitespace and edge case handling

### Not Covered (by design)
- ❌ Signal handling (Ctrl-C, Ctrl-D, Ctrl-\\)
- ❌ Interactive mode behavior
- ❌ Command execution (this is lexer/parser testing only)
- ❌ Memory leak testing
- ❌ Performance testing

## 📚 Related Documentation
- `../BUG_REPORT.md` - Detailed bug analysis
- `../doc/subject.txt` - Project requirements
- `../CLAUDE.md` - Development guidelines
- `../Makefile` - Build configuration