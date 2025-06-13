/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_redirection_executor.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/14 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_executor.h"

t_cmd_redirect	*create_test_redirect(t_redirect_type type, const char *file)
{
	t_cmd_redirect	*redirect;

	redirect = malloc(sizeof(t_cmd_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->file = strdup(file);
	redirect->fd = -1;
	redirect->next = NULL;
	return (redirect);
}

void	test_output_redirection(void)
{
	printf("Test: output redirection... ");
	
	char *argv[] = {"echo", "test output", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	cmd->redirects = create_test_redirect(REDIRECT_OUTPUT, "test_output.txt");
	
	t_exec_context *ctx = create_test_context();
	
	int result = execute_single_command(cmd, ctx);
	assert(result == EXIT_SUCCESS);
	
	// Verify file was created
	FILE *file = fopen("test_output.txt", "r");
	assert(file != NULL);
	
	char buffer[256];
	fgets(buffer, sizeof(buffer), file);
	assert(strstr(buffer, "test output") != NULL);
	fclose(file);
	
	printf("PASSED\n");
	
	unlink("test_output.txt");
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_input_redirection(void)
{
	printf("Test: input redirection... ");
	
	// Create input file
	FILE *input_file = fopen("test_input.txt", "w");
	fprintf(input_file, "input line 1\ninput line 2\n");
	fclose(input_file);
	
	char *argv[] = {"cat", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	cmd->redirects = create_test_redirect(REDIRECT_INPUT, "test_input.txt");
	
	t_exec_context *ctx = create_test_context();
	
	int result = execute_single_command(cmd, ctx);
	assert(result == EXIT_SUCCESS);
	
	printf("PASSED\n");
	
	unlink("test_input.txt");
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_append_redirection(void)
{
	printf("Test: append redirection... ");
	
	// Create initial file
	FILE *file = fopen("test_append.txt", "w");
	fprintf(file, "initial content\n");
	fclose(file);
	
	char *argv[] = {"echo", "appended content", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	cmd->redirects = create_test_redirect(REDIRECT_APPEND, "test_append.txt");
	
	t_exec_context *ctx = create_test_context();
	
	int result = execute_single_command(cmd, ctx);
	assert(result == EXIT_SUCCESS);
	
	// Verify content was appended
	file = fopen("test_append.txt", "r");
	assert(file != NULL);
	
	char buffer[256];
	int line_count = 0;
	while (fgets(buffer, sizeof(buffer), file))
		line_count++;
	assert(line_count == 2);  // Should have 2 lines now
	
	fclose(file);
	printf("PASSED\n");
	
	unlink("test_append.txt");
	free_cmd(cmd);
	free_exec_context(ctx);
}

void	test_multiple_redirections(void)
{
	printf("Test: multiple redirections... ");
	
	// Create input file
	FILE *input_file = fopen("test_multi_input.txt", "w");
	fprintf(input_file, "multi redirection test\n");
	fclose(input_file);
	
	char *argv[] = {"cat", NULL};
	t_cmd *cmd = create_test_cmd(argv);
	
	// Add input redirection
	cmd->redirects = create_test_redirect(REDIRECT_INPUT, "test_multi_input.txt");
	// Add output redirection
	cmd->redirects->next = create_test_redirect(REDIRECT_OUTPUT, "test_multi_output.txt");
	
	t_exec_context *ctx = create_test_context();
	
	int result = execute_single_command(cmd, ctx);
	assert(result == EXIT_SUCCESS);
	
	// Verify output file contains input content
	FILE *output_file = fopen("test_multi_output.txt", "r");
	assert(output_file != NULL);
	
	char buffer[256];
	fgets(buffer, sizeof(buffer), output_file);
	assert(strstr(buffer, "multi redirection test") != NULL);
	fclose(output_file);
	
	printf("PASSED\n");
	
	unlink("test_multi_input.txt");
	unlink("test_multi_output.txt");
	free_cmd(cmd);
	free_exec_context(ctx);
}

int	main(void)
{
	printf("=== Executor Tests: Redirection ===\n");
	
	test_output_redirection();
	test_input_redirection();
	test_append_redirection();
	test_multiple_redirections();
	
	printf("\nAll redirection tests passed!\n");
	cleanup_test_files();
	return (0);
}