# Minishell

A simple shell implementation as part of the 42 school curriculum. This project recreates the functionality of bash, implementing core shell features including command execution, pipes, redirections, and built-in commands.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Supported Features](#supported-features)
- [Project Status](#project-status)
- [Testing](#testing)
- [Architecture](#architecture)

## Overview

Minishell is a simplified version of bash that handles basic shell operations. It combines components from two implementations:
- **Lexer & Builtins**: Complete tokenization and built-in command implementations
- **Parser**: Advanced recursive descent parser for complex command structures

## Features

### ✅ Implemented
- **Prompt Display**: Interactive command prompt
- **Command History**: Working command history using readline
- **Built-in Commands**: All 7 required commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
- **Lexical Analysis**: Complete tokenization of input including quotes and redirections
- **Parsing**: Advanced AST generation for complex command structures
- **Environment Variables**: Complete environment variable management
- **Quote Handling**: Single and double quote processing

### 🚧 In Progress
- **Command Execution**: Currently only built-ins work
- **Pipes**: Parsed but not executed
- **Redirections**: Parsed but not executed
- **Heredoc**: Parser support exists, execution pending

### ❌ Not Implemented
- **Signal Handling**: Ctrl+C, Ctrl+D, Ctrl+\ behavior
- **Operators**: `&&`, `||`, `;` (lexer support needed)
- **External Commands**: PATH-based command execution

## Requirements

- **OS**: Unix-like systems (Linux/macOS)
- **Compiler**: gcc with flags `-Wall -Wextra -Werror`
- **Libraries**: readline library
- **Standard**: C99 compliant, follows 42 Norm

## Installation

```bash
# Clone the repository
git clone [repository-url]
cd minishell

# Build the project
make

# Clean build files
make clean      # Remove object files
make fclean     # Remove all generated files
make re         # Rebuild everything
```

## Usage

### Basic Usage

```bash
# Start the shell
./minishell

# Example commands (built-ins only currently work)
minishell$ echo "Hello World"
minishell$ pwd
minishell$ env
minishell$ export TEST_VAR=value
minishell$ echo $TEST_VAR
minishell$ exit
```

### Testing

```bash
# Run manual tests
./run_test.sh

# Run parser tests
make test_parser

# Run individual parser tests
make test_simple_command
make test_quote_handling
make test_pipe
make test_redirection
```

## Built-in Commands

| Command | Description | Status |
|---------|-------------|--------|
| `echo [-n]` | Display text, -n option suppresses newline | ✅ |
| `cd [path]` | Change directory (relative/absolute paths) | ✅ |
| `pwd` | Print working directory | ✅ |
| `export [var=value]` | Set environment variables | ✅ |
| `unset [var]` | Remove environment variables | ✅ |
| `env` | Display environment variables | ✅ |
| `exit [code]` | Exit shell with optional exit code | ✅ |

## Supported Features

### Quote Handling
- **Single quotes (`'`)**: Preserve literal values of all characters
- **Double quotes (`"`)**: Preserve literal values except `$` (variable expansion)

### Redirections (Parsed, Not Executed)
- `<` : Input redirection
- `>` : Output redirection
- `>>` : Append output redirection  
- `<<` : Heredoc (delimiter-based input)

### Pipes (Parsed, Not Executed)
- `|` : Connect output of one command to input of next

### Environment Variables
- `$VAR` : Variable expansion
- `$?` : Exit status of last command (planned)

### Signal Handling (Planned)
- `Ctrl+C` : New prompt on new line
- `Ctrl+D` : Exit shell
- `Ctrl+\` : No action

## Project Status

### Current Implementation State

```
Input → Lexer → Tokens → Parser → AST → Executor → Output
         ✅       ✅       ✅      ✅       ❌
```

### Component Status

| Component | Status | Location |
|-----------|--------|----------|
| Lexer | ✅ Complete | `src/usecase/lexer/` |
| Parser | ✅ Complete | `src/usecase/parser/` |
| Built-ins | ✅ Complete | `src/usecase/builtin/` |
| Environment | ✅ Complete | `src/domain/env/` |
| Executor | ❌ Missing | Needs implementation |
| Signals | ❌ Missing | Needs implementation |

### Known Limitations

1. **Execution**: Only built-in commands execute; external commands are parsed but not run
2. **Operators**: `&&`, `||`, `;` need lexer support
3. **Pipes/Redirections**: Fully parsed, awaiting executor implementation
4. **Signals**: No signal handling implemented yet

## Testing

### Manual Testing Commands

```bash
# Working (built-ins)
echo hello world
pwd
env | grep PATH  # Note: pipes parsed but not executed

# Parsed but not executed
ls | grep main
echo hello > file.txt
cat < input.txt
echo -e "line1\nline2" | wc -l
```

### Test Structure

- **Lexer Tests**: `tests/lexer/` - Complete tokenization testing
- **Parser Tests**: `tests/parser/` - AST generation testing
- **Builtin Tests**: `tests/builtin/` - Built-in command testing

## Architecture

### Clean Architecture Design

```
minishell/
├── include/           # Headers
│   ├── domain/        # Domain models
│   ├── usecase/       # Use case interfaces
│   └── utils/         # Utility headers
├── src/
│   ├── domain/        # Core business logic
│   ├── usecase/       # Application logic
│   │   ├── lexer/     # Tokenization
│   │   ├── parser/    # AST generation
│   │   ├── builtin/   # Built-in commands
│   │   └── exit/      # Exit handling
│   ├── adapters/      # Interface adapters
│   │   └── cli/       # Command line interface
│   └── utils/         # Shared utilities
└── tests/             # Test suites
```

### Data Flow

1. **Input** → User command line input
2. **Lexer** → Tokenizes input into structured tokens
3. **Parser** → Builds Abstract Syntax Tree from tokens
4. **Executor** → Executes AST (TO BE IMPLEMENTED)
5. **Output** → Command results

## Development

### Adding New Features

1. **Built-in Commands**: Add to `src/usecase/builtin/`
2. **Lexer Handlers**: Add to `src/usecase/lexer/handler/`
3. **Parser Extensions**: Modify `src/usecase/parser/`

### Code Style

- Follows 42 Norm standards
- Functions ≤ 25 lines
- ≤ 5 functions per file
- ≤ 4 parameters per function
- Use `t_` prefix for types

### Memory Management

- All heap allocations must be freed
- No memory leaks tolerated
- Use valgrind for leak detection

## Contributing

This is a 42 school project. Please follow the project guidelines and 42 Norm standards for any contributions.

## License

This project is part of the 42 school curriculum.