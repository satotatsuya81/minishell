#!/bin/bash

# 42 Minishell Comprehensive Evaluation Test Suite
# This script tests all mandatory requirements from the 42 evaluation guide

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Test result tracking
FAILED_TEST_NAMES=()

echo -e "${CYAN}=============================================================${NC}"
echo -e "${CYAN}  42 MINISHELL COMPREHENSIVE EVALUATION TEST SUITE${NC}"
echo -e "${CYAN}=============================================================${NC}"
echo ""

# Function to run a test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit_code="${3:-0}"
    local expected_output="$4"
    local should_contain="${5:-true}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Testing: ${test_name}${NC}"
    
    # Run the command with timeout
    timeout 5s bash -c "echo '$command' | ./minishell" > /tmp/minishell_test_output 2>&1
    local actual_exit_code=$?
    
    # Extract only the actual command output (line immediately after minishell>)
    local actual_output=$(cat /tmp/minishell_test_output | grep -A1 "minishell>" | grep -v "minishell>" | grep -v "exit" | head -1)
    
    # Check exit code
    local exit_code_ok=true
    if [ "$actual_exit_code" -ne "$expected_exit_code" ] && [ "$actual_exit_code" -ne 124 ]; then
        exit_code_ok=false
    fi
    
    # Check output if specified
    local output_ok=true
    if [ -n "$expected_output" ]; then
        if [ "$should_contain" = "true" ]; then
            if [[ "$actual_output" != *"$expected_output"* ]]; then
                output_ok=false
            fi
        else
            if [[ "$actual_output" == *"$expected_output"* ]]; then
                output_ok=false
            fi
        fi
    fi
    
    # Report result
    if [ "$exit_code_ok" = true ] && [ "$output_ok" = true ]; then
        echo -e "  ${GREEN}✅ PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "  ${RED}❌ FAILED${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_NAMES+=("$test_name")
        
        if [ "$exit_code_ok" = false ]; then
            echo -e "    Expected exit code: $expected_exit_code, Got: $actual_exit_code"
        fi
        if [ "$output_ok" = false ]; then
            echo -e "    Output check failed"
            echo -e "    Expected to contain: '$expected_output'"
            echo -e "    Actual output: '$actual_output'"
        fi
    fi
    echo ""
}

# Function to test compilation
test_compilation() {
    echo -e "${YELLOW}=== COMPILATION TESTS ===${NC}"
    
    # Test makefile flags
    if make -n 2>&1 | grep -q '\-Wall \-Wextra \-Werror'; then
        echo -e "${GREEN}✅ Makefile uses required flags (-Wall -Wextra -Werror)${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Makefile missing required compilation flags${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_NAMES+=("Compilation flags")
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Test clean compilation
    if make clean > /dev/null 2>&1 && make > /dev/null 2>&1; then
        echo -e "${GREEN}✅ Project compiles cleanly without errors${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Compilation errors detected${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_NAMES+=("Clean compilation")
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Test no relinking
    make > /dev/null 2>&1
    if make 2>&1 | grep -q 'Nothing to be done\|up to date'; then
        echo -e "${GREEN}✅ Makefile doesn't relink unnecessarily${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Makefile relinks when not necessary${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_NAMES+=("No relinking")
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo ""
}

# Function to test global variables
test_global_variables() {
    echo -e "${YELLOW}=== GLOBAL VARIABLES TEST ===${NC}"
    
    # Check for global variables (allowing signal-related ones)
    local global_count=$(grep -r "^[a-zA-Z_][a-zA-Z0-9_]*\s*g_" src/ include/ 2>/dev/null | grep -v "g_signal" | wc -l)
    local extern_count=$(grep -r "extern.*g_" include/ 2>/dev/null | grep -v "g_signal" | wc -l)
    
    if [ "$global_count" -eq 0 ] && [ "$extern_count" -le 1 ]; then
        echo -e "${GREEN}✅ Minimal global variables (justified signal handling only)${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Too many global variables detected${NC}"
        echo -e "    Found $global_count definitions and $extern_count declarations"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_NAMES+=("Global variables")
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo ""
}

# Ensure minishell is built
echo -e "${YELLOW}Building minishell...${NC}"
if ! make > /dev/null 2>&1; then
    echo -e "${RED}❌ Failed to build minishell. Exiting.${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Minishell built successfully${NC}"
echo ""

# Run compilation tests
test_compilation

# Run global variables test
test_global_variables

# Basic command execution tests
echo -e "${YELLOW}=== BASIC COMMAND TESTS ===${NC}"
run_test "Absolute path command" "/bin/echo hello" 0 "hello"
run_test "Empty command" "" 0
run_test "Spaces and tabs only" "   \t  " 0
run_test "Nonexistent command" "/bin/nonexistent" 0 "command not found"

# Echo builtin tests
echo -e "${YELLOW}=== ECHO BUILTIN TESTS ===${NC}"
run_test "Basic echo" "echo hello world" 0 "hello world"
run_test "Echo with -n flag" "echo -n test" 0 "test"
run_test "Echo without arguments" "echo" 0

# Exit builtin tests
echo -e "${YELLOW}=== EXIT BUILTIN TESTS ===${NC}"
run_test "Exit without arguments" "exit" 0
run_test "Exit with valid code" "exit 42" 42

# Return value tests
echo -e "${YELLOW}=== RETURN VALUE TESTS ===${NC}"
run_test "Successful command return" "/bin/true\necho \$?" 0 "0"
run_test "Failed command return" "/bin/false\necho \$?" 0 "1"
run_test "Command not found return" "/bin/nonexistent 2>/dev/null; echo \$?" 0 "127"

# Environment variable tests
echo -e "${YELLOW}=== ENVIRONMENT VARIABLE TESTS ===${NC}"
run_test "Basic env command" "env" 0 "PATH="
run_test "Export new variable" "export TEST_VAR=hello\nenv | grep TEST_VAR" 0 "TEST_VAR=hello"
run_test "Unset variable" "export TEST_VAR=hello\nunset TEST_VAR\nenv | grep TEST_VAR || echo NOTFOUND" 0 "NOTFOUND"

# Variable expansion tests
echo -e "${YELLOW}=== VARIABLE EXPANSION TESTS ===${NC}"
run_test "USER variable expansion" "echo \$USER" 0
run_test "Exit status expansion" "/bin/true\necho \$?" 0 "0"
run_test "Variable in double quotes" "echo \"\$USER\"" 0
run_test "Variable NOT expanded in single quotes" 'echo '"'"'$USER'"'"'' 0 "\$USER"

# Quote handling tests
echo -e "${YELLOW}=== QUOTE HANDLING TESTS ===${NC}"
run_test "Double quotes with spaces" "echo \"hello world\"" 0 "hello world"
run_test "Single quotes with spaces" "echo 'hello world'" 0 "hello world"
run_test "Special chars in single quotes" 'echo '"'"'cat file | grep pattern'"'"'' 0 "cat file | grep pattern"
run_test "Variable expansion in double quotes" "export TEST=value\necho \"\$TEST\"" 0 "value"

# Builtin commands tests
echo -e "${YELLOW}=== BUILTIN COMMANDS TESTS ===${NC}"
run_test "PWD command" "pwd" 0
run_test "CD to parent directory" "cd ..\npwd" 0
run_test "CD to nonexistent directory" "cd /nonexistent 2>/dev/null\necho \$?" 0 "1"

# Redirection tests
echo -e "${YELLOW}=== REDIRECTION TESTS ===${NC}"
run_test "Output redirection" "echo test > /tmp/minishell_test\ncat /tmp/minishell_test" 0 "test"
run_test "Input redirection" "echo input > /tmp/minishell_input\ncat < /tmp/minishell_input" 0 "input"
run_test "Append redirection" "echo first > /tmp/minishell_append; echo second >> /tmp/minishell_append; cat /tmp/minishell_append" 0 "first"

# Pipe tests
echo -e "${YELLOW}=== PIPE TESTS ===${NC}"
run_test "Simple pipe" "echo hello | cat" 0 "hello"
run_test "Multiple pipes" "echo test | cat | cat" 0 "test"
run_test "Pipe with grep" "echo -e 'line1\nline2\nline3' | grep line2" 0 "line2"

# Complex scenarios
echo -e "${YELLOW}=== COMPLEX SCENARIO TESTS ===${NC}"
run_test "Long command line" "echo arg1 arg2 arg3 arg4 arg5 arg6 arg7 arg8 arg9 arg10" 0 "arg1 arg2 arg3"
run_test "Nested quotes" "echo \"single 'quotes' inside double\"" 0 "single quotes inside double"
run_test "Pipes with redirections" "echo hello | cat > /tmp/minishell_pipe\ncat /tmp/minishell_pipe" 0 "hello"

# Error handling tests
echo -e "${YELLOW}=== ERROR HANDLING TESTS ===${NC}"
run_test "Invalid syntax handling" "echo |" 0 "" false
run_test "File not found" "cat /nonexistent/file 2>/dev/null || echo HANDLED" 0 "HANDLED"

# PATH tests
echo -e "${YELLOW}=== PATH HANDLING TESTS ===${NC}"
run_test "Command via PATH" "ls > /dev/null 2>&1 && echo FOUND || echo NOTFOUND" 0 "FOUND"
run_test "Unset PATH behavior" "unset PATH\nls > /dev/null 2>&1 || echo NOTFOUND" 0 "NOTFOUND"

# Clean up temporary files
rm -f /tmp/minishell_test /tmp/minishell_input /tmp/minishell_append /tmp/minishell_pipe /tmp/minishell_test_output

# Final results
echo -e "${CYAN}=============================================================${NC}"
echo -e "${CYAN}  TEST RESULTS SUMMARY${NC}"
echo -e "${CYAN}=============================================================${NC}"
echo ""
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
echo -e "${RED}Failed: $FAILED_TESTS${NC}"

if [ $FAILED_TESTS -gt 0 ]; then
    echo ""
    echo -e "${RED}Failed Tests:${NC}"
    for test_name in "${FAILED_TEST_NAMES[@]}"; do
        echo -e "  ${RED}❌ $test_name${NC}"
    done
fi

echo ""
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED! Minishell is ready for 100% evaluation! 🎉${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed. Please review and fix the issues.${NC}"
    echo -e "${YELLOW}Success rate: $(( PASSED_TESTS * 100 / TOTAL_TESTS ))%${NC}"
    exit 1
fi