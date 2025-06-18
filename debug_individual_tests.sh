#!/bin/bash

echo "=== Testing Command Not Found Exit Code ==="
echo "Command: /bin/nonexistent"
echo "/bin/nonexistent" | ./minishell > /tmp/test_output 2>&1
exit_code=$?
echo "Exit code: $exit_code"
echo "Output:"
cat /tmp/test_output
echo ""

echo "=== Testing Multiple Commands ==="
echo "Commands: echo first; echo second"
echo -e "echo first\necho second" | ./minishell > /tmp/test_output 2>&1
exit_code=$?
echo "Exit code: $exit_code"
echo "Output:"
cat /tmp/test_output
echo ""

echo "=== Testing Append Redirection ==="
echo "Command: echo first > /tmp/test; echo second >> /tmp/test; cat /tmp/test"
echo -e "echo first > /tmp/test\necho second >> /tmp/test\ncat /tmp/test" | ./minishell > /tmp/test_output 2>&1
exit_code=$?
echo "Exit code: $exit_code"
echo "Output:"
cat /tmp/test_output
echo "File content:"
if [ -f /tmp/test ]; then
    cat /tmp/test
else
    echo "File not created"
fi
echo ""

# Cleanup
rm -f /tmp/test_output /tmp/test