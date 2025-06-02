# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a minishell project (42 school curriculum) that integrates components from two implementations:
- **minishell_tatsato**: Original implementation with complete lexer, builtins, and environment management
- **minishell65**: Advanced parser implementation integrated into this project

## Current Integration Status (as of 2025/01/06)

### ✅ Completed Components

1. **Lexer** (from tatsato)
   - Complete tokenization of input
   - Handles: words, pipes, redirections, quotes, assignments
   - Location: `src/usecase/lexer/`
   - ⚠️ Missing: Operator recognition (`&&`, `||`, `;`)

2. **Parser** (from minishell65)
   - Advanced recursive descent parser
   - Fully implemented: pipes, redirections, operators (&&, ||, ;), heredoc
   - Location: `src/adapters/parser/`
   - Creates complete AST (Abstract Syntax Tree)
   - ⚠️ Note: Parser can handle all operators but lexer doesn't tokenize them yet

3. **Builtins** (from tatsato)
   - All 7 required commands implemented: `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`
   - Location: `src/usecase/builtin/`

4. **Environment Management** (from tatsato)
   - Complete environment variable handling
   - Location: `src/domain/env/`

### ❌ Missing Components

1. **Executor**
   - No implementation for executing parsed AST
   - Needed for: pipes, redirections, external commands
   - Current: Only simple builtin execution in main.c

2. **Operator Lexing**
   - Lexer doesn't recognize `&&`, `||`, `;`
   - Parser can handle them if lexer provides TOKEN_OPERATOR

3. **Token Implementation**
   - `src/entities/token.c` is missing (may contain token manipulation functions)
   - Need to verify if this functionality exists elsewhere or needs to be implemented

## Build Commands

```bash
make                    # Build minishell executable
make clean             # Remove object files
make fclean            # Full clean including executable
make re                # Rebuild everything
make test_parser       # Run parser unit tests (from minishell65)
```

## Architecture

### Current Directory Structure
```
minishell_tatsato/
├── include/
│   ├── domain/         # Domain models (token types, env vars)
│   ├── entities/       # Command and pipeline structures
│   ├── interfaces/     # Parser interfaces
│   └── usecase/        # Use case headers
├── src/
│   ├── adapters/
│   │   ├── cli/        # CLI utilities
│   │   └── parser/     # Parser implementation (from minishell65)
│   ├── domain/
│   │   └── env/        # Environment management
│   ├── entities/       # Entity implementations
│   ├── usecase/
│   │   ├── assignment/ # Variable assignment
│   │   ├── builtin/    # Builtin commands
│   │   ├── exit/       # Exit handling
│   │   └── lexer/      # Tokenization
│   └── utils/
│       ├── libft/      # Standard C library replacements
│       └── libft_custom/ # Project-specific utilities
└── tests/
    ├── builtin/        # Builtin command tests
    ├── lexer/          # Lexer tests
    └── parser/         # Parser tests
```

### Target Directory Structure (Planned Refactoring)
See `doc/minishell-directory-structure.txt` for the planned Clean Architecture-based structure.

### Data Flow
```
Input → Lexer → Tokens → Parser → AST → Executor → Output
         ✓        ✓        ✓       ✓       ❌
```

### Parser Implementation Details

#### AST Structure
```c
// Command structure
typedef struct s_cmd {
    char **argv;                   // Command and arguments array
    struct s_cmd_redirect *redirects;  // Redirection list
    struct s_cmd *next;           // Next command (for pipes)
} t_cmd;

// Redirection structure
typedef struct s_cmd_redirect {
    t_redirect_type type;         // Redirection type
    char *file;                   // File name
    int fd;                       // File descriptor (if needed)
    struct s_cmd_redirect *next;  // Next redirection
} t_cmd_redirect;

// Pipeline structure (command chain)
typedef struct s_pipeline {
    t_cmd *cmds;                  // Command list
    struct s_pipeline *next;      // Next pipeline (operators)
    int connector;                // Connector type (AND, OR, SEMICOLON)
} t_pipeline;
```

#### Parser Interface
```c
// Main parse function
t_pipeline *parse(t_token_stream *tokens);

// Parse result with error info
typedef struct s_parse_result {
    t_pipeline *ast;              // Parsed AST
    char *error_msg;              // Error message
    int error_line;               // Error line
    int error_column;             // Error column
} t_parse_result;
```

#### Parser Status
- ✅ Simple commands
- ✅ Commands with arguments
- ✅ Pipes (single and multiple)
- ✅ Redirections (<, >, >>)
- ✅ Operators (&&, ||, ;)
- 🚧 Heredoc (<<) - In progress
- ❌ Error handling improvements needed

### Key Data Structures

#### Token (tatsato's structure)
```c
typedef struct s_token {
    t_token_type type;
    int length;
    t_value value;      // union: word, assignment, redirect
    int line_number;
    int column;
    struct s_token *next;
} t_token;
```

#### Command/AST (from minishell65)
```c
typedef struct s_cmd {
    char **argv;
    struct s_cmd_redirect *redirects;
    struct s_cmd *next;  // for pipes
} t_cmd;

typedef struct s_pipeline {
    t_cmd *cmds;
    struct s_pipeline *next;
    int connector;  // AND, OR, SEMICOLON
} t_pipeline;
```

## Development Guidelines

### Next Steps Priority

1. **Implement Executor** (High Priority)
   - Create `src/adapters/executor/`
   - Handle pipes with fork/dup2
   - Handle redirections with open/dup2
   - Execute external commands with execve

2. **Add Operator Lexing** (Medium Priority)
   - Update lexer to recognize `&&`, `||`, `;`
   - Add handler in `src/usecase/lexer/handler/`

3. **Signal Handling** (Medium Priority)
   - Implement Ctrl+C, Ctrl+D, Ctrl+\
   - Add to `src/adapters/system/` (planned location)

4. **Missing Parser Tests Migration**
   - Parser tests exist in `tests/parser/` but may need updates
   - Verify all tests compile and pass with current implementation

### Testing Approach

1. **Current Test Status**
   - Lexer tests: Available in `tests/lexer/`
   - Parser tests: Need to be migrated from minishell65
   - Integration: Use `run_test.sh` for manual testing

2. **Test Commands**
   ```bash
   # Working
   echo hello world
   pwd
   env
   
   # Parsed but not executed
   ls | grep main
   echo hello > file.txt
   echo a && echo b  # Note: && not lexed yet
   ```

### Important Notes

1. **42 Norm Compliance**
   - Functions ≤ 25 lines
   - ≤ 5 functions per file
   - ≤ 4 parameters per function
   - Use `t_` prefix for types

2. **Memory Management**
   - All mallocs must be freed
   - Use valgrind for leak checking
   - Parser has cleanup functions: `free_parse_result()`, `free_pipeline()`, etc.

3. **Integration Gotchas**
   - `t_redirect` was renamed to `t_cmd_redirect` to avoid conflicts
   - Parser expects `t_token_stream` from lexer
   - main.c currently has simple executor for builtins only

4. **Branch Management**
   - Working on: `feature/integrate-minishell65-parser`
   - Do NOT modify main branch directly

## Quick Reference

### Add a new builtin
1. Implement in `src/usecase/builtin/ft_newcmd.c`
2. Add prototype to `include/usecase/builtin/builtin_commands.h`
3. Add case in `main.c` executor

### Debug parser issues
1. Check token output in main.c (already prints tokens)
2. Parser result is printed after parsing
3. Add debug prints in parser files if needed

### Test specific functionality
```bash
# Test lexer only
echo "your command" | ./minishell | grep "=== Tokens ==="

# Test parser
echo "your command" | ./minishell | grep "Parse"
```