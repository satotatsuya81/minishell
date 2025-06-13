/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_external_executor.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/14 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_executor.h"

extern char **__environ;

void	test_external_ls(void)
{
	printf("Test: external ls... ");
	
	char *argv[] = {"ls", ".", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_external(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	printf("PASSED\n");
	
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_external_cat(void)
{
	printf("Test: external cat... ");
	
	// Create test file
	FILE *file = fopen("test_cat_input.txt", "w");
	fprintf(file, "test content\n");
	fclose(file);
	
	char *argv[] = {"cat", "test_cat_input.txt", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_external(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	printf("PASSED\n");
	
	unlink("test_cat_input.txt");
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_external_invalid_command(void)
{
	printf("Test: invalid external command... ");
	
	char *argv[] = {"nonexistent_command_12345", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_external(cmd, ctx);
	
	assert(result == EXIT_COMMAND_NOT_FOUND);
	printf("PASSED\n");
	
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_external_echo_system(void)
{
	printf("Test: external echo (system command)... ");
	
	char *argv[] = {"/bin/echo", "system", "echo", "test", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	t_exec_context *ctx = create_test_context();
	
	int result = execute_external(cmd, ctx);
	
	assert(result == EXIT_SUCCESS);
	printf("PASSED\n");
	
	free_cmd(cmd);
	free_exec_context(ctx);
}

int	main(void)
{
	printf("=== Executor Tests: External Commands ===\n");
	
	test_external_ls();
	test_external_cat();
	test_external_invalid_command();
	test_external_echo_system();
	
	printf("\nAll external command tests passed!\n");
	return (0);
}