/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_simple_executor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/14 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "usecase/executor/executor.h"
#include "domain/command.h"

extern char **__environ;

/* Simple test utilities */
t_cmd	*create_simple_cmd(char *cmd_name, char *arg)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	
	cmd->argv = malloc(sizeof(char *) * 3);
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	
	cmd->argv[0] = strdup(cmd_name);
	if (arg)
		cmd->argv[1] = strdup(arg);
	else
		cmd->argv[1] = NULL;
	cmd->argv[2] = NULL;
	
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	free_simple_cmd(t_cmd *cmd)
{
	if (!cmd)
		return;
	if (cmd->argv)
	{
		free(cmd->argv[0]);
		if (cmd->argv[1])
			free(cmd->argv[1]);
		free(cmd->argv);
	}
	free(cmd);
}

void	test_is_builtin_function(void)
{
	printf("Test: is_builtin function... ");
	
	assert(is_builtin("echo") == 1);
	assert(is_builtin("pwd") == 1);
	assert(is_builtin("cd") == 1);
	assert(is_builtin("env") == 1);
	assert(is_builtin("exit") == 1);
	assert(is_builtin("export") == 1);
	assert(is_builtin("unset") == 1);
	
	assert(is_builtin("ls") == 0);
	assert(is_builtin("cat") == 0);
	assert(is_builtin("grep") == 0);
	assert(is_builtin("nonexistent") == 0);
	
	printf("PASSED\n");
}

void	test_simple_echo(void)
{
	printf("Test: simple echo execution... ");
	
	t_cmd *cmd = create_simple_cmd("echo", "test");
	t_exec_context *ctx = create_exec_context(NULL);
	
	if (!cmd || !ctx)
	{
		printf("FAILED - Setup failed\n");
		return;
	}
	
	int result = execute_builtin(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	printf("PASSED\n");
	
	free_simple_cmd(cmd);
	free_exec_context(ctx);
}

void	test_simple_pwd(void)
{
	printf("Test: simple pwd execution... ");
	
	t_cmd *cmd = create_simple_cmd("pwd", NULL);
	t_exec_context *ctx = create_exec_context(NULL);
	
	if (!cmd || !ctx)
	{
		printf("FAILED - Setup failed\n");
		return;
	}
	
	int result = execute_builtin(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	printf("PASSED\n");
	
	free_simple_cmd(cmd);
	free_exec_context(ctx);
}

int	main(void)
{
	printf("=== Simple Executor Tests ===\n");
	
	test_is_builtin_function();
	test_simple_echo();
	test_simple_pwd();
	
	printf("\nSimple executor tests completed!\n");
	return (0);
}