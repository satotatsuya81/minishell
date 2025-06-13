/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_executor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/14 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_executor.h"

/* Test utilities */
t_cmd	*create_test_cmd(char **argv)
{
	t_cmd	*cmd;
	int		i;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *) * 10);
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	while (argv[i])
	{
		cmd->argv[i] = strdup(argv[i]);
		i++;
	}
	cmd->argv[i] = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_exec_context	*create_test_context(void)
{
	t_env_var	*env;
	
	env = env_create_from_envp(__environ);
	return (create_exec_context(&env));
}

void	cleanup_test_files(void)
{
	unlink("test_output.txt");
	unlink("test_input.txt");
	unlink("test_append.txt");
}

/* Builtin tests */
void	test_builtin_echo(void)
{
	printf("Test: builtin echo... ");
	
	char *argv[] = {"echo", "hello", "world", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_builtin(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	printf("PASSED\n");
	
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_builtin_pwd(void)
{
	printf("Test: builtin pwd... ");
	
	char *argv[] = {"pwd", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_builtin(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	printf("PASSED\n");
	
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_builtin_env(void)
{
	printf("Test: builtin env... ");
	
	char *argv[] = {"env", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_builtin(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	printf("PASSED\n");
	
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_builtin_cd(void)
{
	printf("Test: builtin cd... ");
	
	char *current_dir = getcwd(NULL, 0);
	
	char *argv[] = {"cd", "/tmp", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_builtin(cmd, ctx);
	assert(result == EXIT_SUCCESS);
	
	char *new_dir = getcwd(NULL, 0);
	assert(strcmp(new_dir, "/tmp") == 0);
	
	// Restore original directory
	chdir(current_dir);
	
	printf("PASSED\n");
	
	free(current_dir);
	free(new_dir);
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_builtin_exit(void)
{
	printf("Test: builtin exit... ");
	
	char *argv[] = {"exit", "42", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_builtin(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	assert(ctx->should_exit == 1);
	assert(ctx->exit_code == 42);
	printf("PASSED\n");
	
	free_cmd(cmd);
	free_exec_context(ctx);
}

int	main(void)
{
	printf("=== Executor Tests: Builtin Commands ===\n");
	
	test_builtin_echo();
	test_builtin_pwd();
	test_builtin_env();
	test_builtin_cd();
	test_builtin_exit();
	
	printf("\nAll builtin tests passed!\n");
	return (0);
}