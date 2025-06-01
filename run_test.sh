#!/bin/bash

echo "=== Minishell Parser Integration Test ==="
echo

# Simple commands
echo "Test 1: Simple echo"
echo "echo hello" | ./minishell | head -20

echo -e "\n---\n"

echo "Test 2: PWD command"
echo "pwd" | ./minishell | head -20

echo -e "\n---\n"

echo "Test 3: Pipe command (should show 'not implemented' message)"
echo "ls | grep main" | ./minishell | head -20

echo -e "\n---\n"

echo "Test 4: Redirection (should show 'not implemented' message)"  
echo "echo hello > test.txt" | ./minishell | head -20

echo -e "\n---\n"

echo "Test 5: Exit command"
echo "exit" | ./minishell | head -20