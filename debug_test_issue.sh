#!/bin/bash

echo "=== Debug test script issues ==="

# Test the exact command that the test script uses
echo "Test 1: Direct command"
echo "echo '\$USER'" | ./minishell > /tmp/test1 2>&1
echo "Output:"
cat /tmp/test1 | grep -A1 "minishell>" | grep -v "minishell>" | grep -v "exit" | head -1

echo ""
echo "Test 2: Via bash -c (like test script)"
bash -c "echo 'echo '\''\$USER'\''' | ./minishell" > /tmp/test2 2>&1
echo "Output:"
cat /tmp/test2 | grep -A1 "minishell>" | grep -v "minishell>" | grep -v "exit" | head -1

echo ""
echo "Test 3: Test script command reconstruction"
command="echo '\$USER'"
timeout 5s bash -c "echo '$command' | ./minishell" > /tmp/test3 2>&1
echo "Command: $command"
echo "Output:"
cat /tmp/test3 | grep -A1 "minishell>" | grep -v "minishell>" | grep -v "exit" | head -1

echo ""
echo "Raw file contents:"
echo "=== test1 ==="
cat /tmp/test1 | head -5
echo "=== test2 ==="  
cat /tmp/test2 | head -5
echo "=== test3 ==="
cat /tmp/test3 | head -5