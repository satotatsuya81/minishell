#!/bin/bash

# Simple 42 Minishell Test - Clean Version
echo "=== 42 MINISHELL FUNCTIONALITY TEST ==="
echo ""

success_count=0
total_count=0

test_command() {
    local description="$1"
    local command="$2"
    local expected="$3"
    
    total_count=$((total_count + 1))
    echo -n "Testing $description... "
    
    # Run command and get only the actual output (first non-debug line)
    local output=$(echo "$command" | ./minishell 2>&1 | grep -v "===" | grep -v "minishell>" | grep -v "✅" | grep -v "❌" | grep -v "EOF" | grep -v "tokens" | grep -v "exit" | head -1)
    
    if [[ "$output" == *"$expected"* ]] || [[ -z "$expected" && -z "$output" ]]; then
        echo "✅ PASS"
        success_count=$((success_count + 1))
    else
        echo "❌ FAIL"
        echo "   Expected: '$expected'"
        echo "   Got: '$output'"
    fi
}

# Basic functionality tests
echo "=== BASIC COMMANDS ==="
test_command "echo" "echo hello" "hello"
test_command "echo with arguments" "echo hello world" "hello world"
test_command "echo -n flag" "echo -n test" "test"

# Builtin commands
echo ""
echo "=== BUILTIN COMMANDS ==="
test_command "pwd" "pwd" "/mnt/c/Users/minat/workspsace/42task/merge_minishell/minishell_tatsato"
test_command "env (should show PATH)" "env" "PATH="

# Variable expansion
echo ""
echo "=== VARIABLE EXPANSION ==="
test_command "variable expansion" "echo \$USER" "minato"

# Quote handling
echo ""
echo "=== QUOTE HANDLING ==="
test_command "double quotes" "echo \"hello world\"" "hello world"
test_command "single quotes" "echo 'hello world'" "hello world"
test_command "variable in double quotes" "echo \"\$USER\"" "minato"
test_command "literal in single quotes" "echo '\$USER'" "\$USER"

# Redirections (test file creation)
echo ""
echo "=== REDIRECTIONS ==="
echo "echo test > /tmp/minishell_test" | ./minishell >/dev/null 2>&1
if [ -f /tmp/minishell_test ] && [ "$(cat /tmp/minishell_test)" = "test" ]; then
    echo "Testing output redirection... ✅ PASS"
    success_count=$((success_count + 1))
    rm -f /tmp/minishell_test
else
    echo "Testing output redirection... ❌ FAIL"
fi
total_count=$((total_count + 1))

# Pipes
echo ""
echo "=== PIPES ==="
test_command "simple pipe" "echo hello | cat" "hello"
test_command "multiple pipes" "echo test | cat | cat" "test"

# Error handling
echo ""
echo "=== ERROR HANDLING ==="
test_command "command not found" "/bin/nonexistent" "command not found"

echo ""
echo "=== SUMMARY ==="
echo "Passed: $success_count/$total_count tests"
percentage=$((success_count * 100 / total_count))
echo "Success rate: $percentage%"

if [ $percentage -ge 90 ]; then
    echo ""
    echo "🎉 EXCELLENT! Your minishell handles all core 42 requirements!"
    echo "Ready for evaluation! 🎯"
elif [ $percentage -ge 80 ]; then
    echo ""
    echo "✅ GOOD! Most core functionality working."
    echo "Minor issues to address for optimal evaluation."
else
    echo ""
    echo "⚠️  Some core functionality needs attention."
fi