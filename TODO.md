# Minishell Development TODO

This document tracks the development progress and remaining tasks for the minishell project.

## Current Status (2025/01/06)

### ✅ Completed Features
- [x] **Lexer Implementation** - Complete tokenization system
- [x] **Parser Implementation** - AST generation for all command types
- [x] **Built-in Commands** - All 7 required commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
- [x] **Environment Management** - Complete environment variable handling
- [x] **Quote Handling** - Single and double quote processing
- [x] **Project Structure** - Clean Architecture implementation

## Critical Missing Components

### 🔥 High Priority (Required for Basic Functionality)

1. **[ ] Executor Implementation**
   - **Status**: Not started
   - **Location**: `src/usecase/executor/` (to be created)
   - **Requirements**:
     - [ ] Fork/exec for external commands
     - [ ] PATH resolution for executables
     - [ ] Process management (wait, waitpid)
     - [ ] Exit status handling
   - **Dependencies**: None
   - **Estimated effort**: 3-5 days

2. **[ ] Pipe Execution**
   - **Status**: Parsed but not executed
   - **Requirements**:
     - [ ] Multi-process pipe handling with fork/dup2
     - [ ] Proper file descriptor management
     - [ ] Process chain coordination
   - **Dependencies**: Executor base implementation
   - **Estimated effort**: 2-3 days

3. **[ ] Redirection Execution**
   - **Status**: Parsed but not executed  
   - **Requirements**:
     - [ ] Input redirection (`<`)
     - [ ] Output redirection (`>`)
     - [ ] Append redirection (`>>`)
     - [ ] Heredoc implementation (`<<`)
   - **Dependencies**: Executor base implementation
   - **Estimated effort**: 2-3 days

4. **[ ] Signal Handling**
   - **Status**: Not implemented
   - **Requirements**:
     - [ ] Ctrl+C: Display new prompt on new line
     - [ ] Ctrl+D: Exit shell
     - [ ] Ctrl+\: No action (ignore)
     - [ ] Signal handling during command execution
   - **Dependencies**: None
   - **Estimated effort**: 1-2 days

## Medium Priority (Enhanced Functionality)

### 🟡 Parser/Lexer Improvements

5. **[ ] Operator Lexing**
   - **Status**: Parser ready, lexer missing
   - **Location**: `src/usecase/lexer/handler/`
   - **Requirements**:
     - [ ] `&&` (logical AND) recognition
     - [ ] `||` (logical OR) recognition  
     - [ ] `;` (semicolon) recognition
     - [ ] Update lexer to generate TOKEN_OPERATOR
   - **Dependencies**: None
   - **Estimated effort**: 1 day

6. **[ ] Variable Expansion**
   - **Status**: Partially implemented
   - **Requirements**:
     - [ ] `$VAR` expansion in commands
     - [ ] `$?` exit status expansion
     - [ ] Variable expansion in double quotes
     - [ ] Prevent expansion in single quotes
   - **Dependencies**: Executor (for `$?`)
   - **Estimated effort**: 1-2 days

### 🟡 Error Handling & Robustness

7. **[ ] Error Handling Improvements**
   - **Status**: Basic error handling exists
   - **Requirements**:
     - [ ] Comprehensive error messages
     - [ ] Proper error codes
     - [ ] Graceful failure handling
     - [ ] Memory cleanup on errors
   - **Dependencies**: All components
   - **Estimated effort**: 1-2 days

8. **[ ] Memory Management Audit**
   - **Status**: Ongoing concern
   - **Requirements**:
     - [ ] Valgrind leak testing
     - [ ] Cleanup function implementation
     - [ ] Error path memory management
   - **Dependencies**: Complete implementation
   - **Estimated effort**: 1 day

## Low Priority (Polish & Testing)

### 🟢 Testing & Validation

9. **[ ] Integration Testing**
   - **Status**: Basic tests exist
   - **Requirements**:
     - [ ] End-to-end command testing
     - [ ] Complex pipeline testing
     - [ ] Error condition testing
     - [ ] Performance testing
   - **Dependencies**: Complete implementation
   - **Estimated effort**: 2-3 days

10. **[ ] Bash Compatibility Testing**
    - **Status**: Not started
    - **Requirements**:
      - [ ] Compare output with bash
      - [ ] Edge case behavior matching
      - [ ] Error message consistency
    - **Dependencies**: Complete implementation
    - **Estimated effort**: 1-2 days

## Bonus Features (Optional)

### ⭐ Advanced Features

11. **[ ] Logical Operators**
    - **Status**: Parsed, not executed
    - **Requirements**:
      - [ ] `&&` conditional execution
      - [ ] `||` conditional execution
      - [ ] Parenthesis support for priorities
    - **Dependencies**: Operator lexing, executor
    - **Estimated effort**: 2-3 days

12. **[ ] Wildcards**
    - **Status**: Not implemented
    - **Requirements**:
      - [ ] `*` wildcard expansion
      - [ ] Current working directory support
      - [ ] Pattern matching
    - **Dependencies**: Complete base implementation
    - **Estimated effort**: 2-3 days

## Development Roadmap

### Phase 1: Core Execution (1-2 weeks)
1. Implement basic executor
2. Add external command execution
3. Implement signal handling

### Phase 2: Advanced Features (1 week)  
1. Implement pipe execution
2. Implement redirection execution
3. Add operator lexing

### Phase 3: Polish & Testing (1 week)
1. Variable expansion
2. Error handling improvements
3. Comprehensive testing

### Phase 4: Bonus Features (Optional)
1. Logical operators
2. Wildcards

## Technical Debt

### Code Quality Issues
- [ ] **Parser Error Handling**: Improve error messages and recovery
- [ ] **Token Management**: Review token lifecycle and cleanup
- [ ] **Code Documentation**: Add function-level documentation
- [ ] **Test Coverage**: Increase test coverage for edge cases

### Architecture Improvements
- [ ] **Executor Interface**: Design clean executor interface
- [ ] **Error Propagation**: Implement consistent error handling pattern
- [ ] **Resource Management**: Centralize resource cleanup

## Clean Architecture Refactoring

### 🔥 Critical Issues (Immediate Action Required)

1. **[ ] Domain Layer Purification**
   - **Issue**: Domain layer depends on external utilities
   - **Location**: `src/domain/env/env_initializer.c:16` includes `utils/libft_custom.h`
   - **Action**: Move implementation to usecase layer, keep only pure entities in domain
   - **Priority**: High
   - **Estimated effort**: 1 day

2. **[ ] Parser Layer Misplacement**
   - **Issue**: Parser logic incorrectly placed in usecase layer
   - **Location**: `src/usecase/parser/` → should be `src/adapters/parser/`
   - **Action**: Move parser implementation to adapters layer
   - **Priority**: High
   - **Estimated effort**: 1-2 days

3. **[ ] Circular Dependencies Resolution**
   - **Issue**: Domain entities have circular references
   - **Location**: `include/domain/env_variable.h:16` includes `domain/assignment.h`
   - **Action**: Remove direct dependencies between domain entities
   - **Priority**: High
   - **Estimated effort**: 1 day

### 🟡 Structural Issues (Medium Priority)

4. **[ ] Layer Responsibility Separation**
   - **Issue**: Business logic scattered across multiple layers
   - **Locations**: Environment logic in both `src/domain/env/` and `src/usecase/builtin/`
   - **Action**: Consolidate business logic in domain services
   - **Priority**: Medium
   - **Estimated effort**: 2-3 days

5. **[ ] Interface Abstraction Improvement**
   - **Issue**: Concrete implementations exposed at interface level
   - **Location**: `include/usecase/executor/executor.h` mixes high/low level functions
   - **Action**: Define proper abstraction interfaces for each layer
   - **Priority**: Medium
   - **Estimated effort**: 2 days

6. **[ ] Dependency Inversion Implementation**
   - **Issue**: High-level modules depend on low-level details
   - **Location**: Executor directly depends on command structures
   - **Action**: Introduce interfaces and dependency injection
   - **Priority**: Medium
   - **Estimated effort**: 2-3 days

### 🟢 Structural Enhancements (Low Priority)

7. **[ ] Infrastructure Layer Implementation**
   - **Issue**: External dependencies not properly abstracted
   - **Location**: Direct system calls scattered throughout codebase
   - **Action**: Create infrastructure layer for filesystem, process management
   - **Priority**: Low
   - **Estimated effort**: 3-4 days

8. **[ ] Testing Architecture Alignment**
   - **Issue**: Test structure doesn't reflect Clean Architecture layers
   - **Location**: `tests/` directory structure
   - **Action**: Reorganize tests by architectural layers
   - **Priority**: Low
   - **Estimated effort**: 1-2 days

### Recommended Directory Restructure

**Current Problem Structure:**
```
src/
├── adapters/cli/          # ✓ Correct
├── domain/env/            # ❌ Contains implementation details
├── usecase/parser/        # ❌ Parser should be in adapters
├── usecase/executor/      # ❌ Mixed concerns
└── utils/                 # ❌ Domain depends on this
```

**Target Clean Architecture:**
```
src/
├── entities/              # Pure domain entities
│   ├── token.c
│   ├── command.c
│   └── assignment.c
├── domain/                # Domain services (business logic)
│   ├── env_service.c
│   └── command_service.c
├── usecase/               # Application business rules
│   ├── shell_interaction.c
│   ├── command_execution.c
│   └── env_management.c
├── adapters/              # Interface adapters
│   ├── cli/
│   ├── parser/            # Move from usecase
│   └── executor/          # Implementation details
└── infrastructure/        # Frameworks & drivers
    ├── filesystem/
    ├── process/
    └── terminal/
```

### Implementation Strategy

**Phase 1: Critical Fixes (Week 1)**
1. Remove domain dependencies on utilities
2. Move parser to adapters layer
3. Resolve circular dependencies

**Phase 2: Structural Improvements (Week 2-3)**
4. Separate business logic concerns
5. Improve interface abstractions
6. Implement dependency inversion

**Phase 3: Complete Restructure (Week 4-5)**
7. Implement infrastructure layer
8. Align testing architecture
9. Documentation updates

## Notes

### 42 Norm Compliance
- All code must follow 42 coding standards
- Functions ≤ 25 lines
- ≤ 5 functions per file  
- ≤ 4 parameters per function

### Memory Management
- No memory leaks allowed
- All malloc calls must have corresponding free
- Use valgrind for verification

### External Functions Allowed
```
readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, 
add_history, printf, malloc, free, write, access, open, read, close, fork, 
wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, 
kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, 
pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, 
ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, 
tgetstr, tgoto, tputs
```

## Progress Tracking

### Core Functionality
- **Total Tasks**: 12 core + 2 bonus = 14 tasks
- **Completed**: 6 tasks (43%)
- **In Progress**: 0 tasks
- **Remaining**: 8 core tasks (57%)

### Clean Architecture Refactoring
- **Total Tasks**: 8 refactoring tasks
- **Critical Issues**: 3 tasks (High Priority)
- **Structural Issues**: 3 tasks (Medium Priority)
- **Enhancements**: 2 tasks (Low Priority)

**Estimated completion**: 
- Core functionality: 3-4 weeks
- Clean Architecture refactoring: 4-5 weeks (can be done in parallel)

---

*Last updated: 2025/01/06*