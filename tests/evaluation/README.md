# 42 Minishell Comprehensive Evaluation Test Suite

## Overview

This test suite provides **complete coverage** of all mandatory requirements from the 42 School minishell evaluation guide. It is designed to ensure your minishell implementation achieves a **100% evaluation score** by testing every requirement rigorously.

## 🎯 Test Coverage

### Mandatory Features (100% Coverage)
- ✅ **Compilation Requirements** - Makefile flags, clean compilation, no relinking
- ✅ **Simple Commands** - Absolute paths, empty commands, whitespace handling
- ✅ **Global Variables** - Zero global variables (Clean Architecture compliance)
- ✅ **Arguments & History** - Command arguments, multiple parameters
- ✅ **Echo Builtin** - All flags (-n), edge cases, no arguments
- ✅ **Exit Builtin** - Valid/invalid codes, error handling
- ✅ **Return Values** - $? expansion, command exit status tracking
- ✅ **Signal Handling** - Ctrl+C, Ctrl+D, Ctrl+\ behavior
- ✅ **Double Quotes** - Variable expansion, special characters, empty quotes
- ✅ **Single Quotes** - Literal interpretation, no expansion, special chars
- ✅ **Environment (env)** - Display current variables, proper formatting
- ✅ **Export Builtin** - Create/update variables, display all, error cases
- ✅ **Unset Builtin** - Remove variables, multiple unset, nonexistent vars
- ✅ **CD Builtin** - Directory navigation, relative paths, error handling
- ✅ **PWD Builtin** - Current directory display, after navigation
- ✅ **Relative Paths** - Complex navigation (../../dir), path resolution
- ✅ **Environment Path** - PATH variable usage, unset PATH handling
- ✅ **Redirections** - Input (<), Output (>), Append (>>), Heredoc (<<)
- ✅ **Pipes** - Simple/complex pipes, error propagation, combined I/O
- ✅ **Complex Scenarios** - Long commands, nested quotes, multiple pipes
- ✅ **Variable Expansion** - $USER, $?, quote context awareness

### Advanced Testing
- ✅ **Edge Cases** - Boundary conditions, extreme inputs, malformed commands
- ✅ **Error Handling** - Syntax errors, permission denied, file not found
- ✅ **Memory Management** - Leak detection, stress testing, rapid execution
- ✅ **Performance** - Command execution speed, memory usage optimization

## 🚀 Running Tests

### Quick Evaluation Check
```bash
make test_quick
```
Runs basic functionality checks to verify core features work.

### Complete Evaluation Suite
```bash
make test_evaluation
```
Runs all 42 evaluation requirements comprehensively.

### All Tests (Unit + Evaluation)
```bash
make test_all
```
Runs both unit tests and evaluation tests.

### Individual Test Categories
```bash
# Build evaluation tests only
make evaluation_tests

# Run directly
./evaluation_tests

# Clean evaluation tests
make clean_eval
```

## 📋 Test Categories

### 1. Mandatory Features (`test_mandatory_features.c`)
- **Compilation**: Makefile compliance, build verification
- **Basic Commands**: Simple execution, argument handling
- **Builtins**: echo, exit with comprehensive parameter testing
- **Return Values**: Exit status tracking and $? expansion

### 2. Signals & Quotes (`test_signals_quotes.c`)
- **Signal Handling**: Interactive mode Ctrl+C/D/\ behavior
- **Signal Safety**: Child process signal propagation
- **Quote Processing**: Single/double quote interpretation
- **Variable Expansion**: Context-aware expansion rules

### 3. Environment & I/O (`test_environment_redirections.c`)
- **Environment Management**: env, export, unset functionality
- **Directory Operations**: cd, pwd with error handling
- **I/O Redirection**: All redirection types with file operations
- **Pipe Operations**: Simple to complex pipeline execution

### 4. Complex & Edge Cases (`test_complex_edge_cases.c`)
- **Variable Expansion**: Multiple variables, nested expansion
- **Path Handling**: Relative/absolute paths, PATH resolution
- **Complex Scenarios**: Long commands, nested structures
- **Edge Cases**: Boundary testing, malformed input
- **Performance**: Memory and execution stress testing

## 🔍 Test Quality Standards

### Rigorous Testing Approach
- **Black Box Testing**: Tests behavior from user perspective
- **Boundary Testing**: Edge cases and limit conditions
- **Error Path Testing**: Invalid inputs and error conditions
- **Integration Testing**: Component interaction verification
- **Performance Testing**: Memory and speed under load

### Evaluation Compliance
- **42 Norm Compliant**: All tests follow 42 School standards
- **Evaluation Scenarios**: Real evaluation testing patterns
- **Score Optimization**: Designed for maximum point achievement
- **Regression Prevention**: Catches implementation regressions

## 🛠️ Test Implementation Details

### Test Architecture
```
tests/evaluation/
├── evaluation_test_suite.h          # Test framework definitions
├── test_utils.c                     # Test execution utilities
├── test_mandatory_features.c        # Core functionality tests
├── test_signals_quotes.c           # Signal and quote handling
├── test_environment_redirections.c  # Environment and I/O tests
├── test_complex_edge_cases.c       # Advanced scenarios
└── evaluation_main.c               # Test suite coordinator
```

### Test Utilities
- **Process Management**: Fork/exec test execution
- **Signal Testing**: Safe signal behavior verification
- **File I/O Testing**: Temporary file creation and cleanup
- **Memory Monitoring**: Leak detection and cleanup verification

### Expected Results
When all tests pass, your minishell implementation:
- ✅ Meets all 42 School mandatory requirements
- ✅ Handles edge cases and error conditions properly
- ✅ Maintains memory safety and performance standards
- ✅ Is ready for 100% evaluation score achievement

## 🎖️ Evaluation Preparation

### Pre-Evaluation Checklist
1. **Run Complete Test Suite**: `make test_evaluation`
2. **Verify Clean Compilation**: `make re`
3. **Check Memory Leaks**: `valgrind ./minishell`
4. **Test Signal Behavior**: Interactive testing
5. **Verify All Builtins**: Manual functionality check

### Common Issues Detected
- Hanging on specific input patterns
- Incorrect signal handling behavior  
- Memory leaks in pipe/redirection operations
- Variable expansion in wrong quote contexts
- Improper exit status propagation

This test suite ensures your minishell implementation is **evaluation-ready** and capable of achieving the **maximum possible score** on the 42 School evaluation.