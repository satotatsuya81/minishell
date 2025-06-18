#!/bin/bash

# Simple test to debug specific issues

echo "=== Testing single quote behavior ==="
echo "Input: echo '\$USER'"
echo "echo '\$USER'" | ./minishell 2>/dev/null | grep -v "===" | grep -v "✅" | grep -v "minishell>" | grep -v "exit" | tail -1

echo ""
echo "=== Testing multiline commands ==="  
echo "Input: echo first\necho second"
echo -e "echo first\necho second" | ./minishell 2>/dev/null | grep -v "===" | grep -v "✅" | grep -v "minishell>" | grep -v "exit"

echo ""
echo "=== Testing command not found ==="
echo "Input: /bin/nonexistent"
echo "/bin/nonexistent" | ./minishell 2>/dev/null | grep -v "===" | grep -v "✅" | grep -v "minishell>" | grep -v "exit"

echo ""
echo "=== Testing redirection parsing ==="
echo "Input: echo test > /tmp/test"
echo "echo test > /tmp/test" | ./minishell 2>/dev/null | grep -v "===" | grep -v "✅" | grep -v "minishell>" | grep -v "exit"
if [ -f /tmp/test ]; then
    echo "File created with content: $(cat /tmp/test)"
    rm -f /tmp/test
else
    echo "File was not created"
fi