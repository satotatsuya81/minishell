/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:17:10 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:32:40 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "domain/token.h"
#include "usecase/env/env_manager.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_printer.h"
#include "usecase/lexer/lexer.h"
#include "usecase/builtin/builtin_commands.h"
#include "adapters/parser/parser_interface.h"
#include "usecase/executor/executor.h"
#include "domain/command.h"
#include "adapters/cli/debug_output.h"
#include "adapters/cli/parser_output.h"
#include "adapters/cli/output_utils.h"
#include "interfaces/io_interface.h"
#include "interfaces/output_interface.h"

static void	execute_and_cleanup(t_parse_result *result, t_exec_context *ctx)
{
	int	status;

	if (result && !result->error_msg && result->ast)
	{
		printf("=== Execution ===\n");
		status = execute_pipeline_list(result->ast, ctx);
		print_execution_summary(status, ctx);
		printf("\n");
		if (ctx->should_exit)
			printf("Exiting minishell...\n");
	}
	else
	{
		printf("=== Execution ===\n");
		printf("❌ Skipped due to parsing errors\n\n");
	}
}

static void	process_and_print(char *line, t_exec_context *exec_ctx)
{
	t_token_stream	*stream;
	t_parse_result	*result;

	stream = lexer(line);
	print_lexer_summary(stream);
	printf("=== Token Details ===\n");
	print_tokens(stream->head);
	printf("\n");
	result = parse(stream);
	print_parse_result(result);
	if (result && result->ast)
	{
		printf("=== AST Details ===\n");
		print_ast(result->ast);
		printf("\n");
	}
	execute_and_cleanup(result, exec_ctx);
	if (result)
		free_parse_result(result);
	free_tokens(stream);
}

static void	process_input_line(char *line, t_exec_context *exec_ctx)
{
	process_and_print(line, exec_ctx);
}

static int	shell_loop(t_exec_context *exec_ctx)
{
	char	*line;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		process_input_line(line, exec_ctx);
		free(line);
		if (exec_ctx->should_exit)
			break ;
	}
	return (exec_ctx->exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_env_var			*env;
	t_exec_context		*exec_ctx;
	t_io_service		*io_service;
	t_output_service	*output_service;
	int					exit_code;

	(void)argc;
	(void)argv;
	io_service = create_io_service();
	output_service = create_output_service();
	if (!io_service || !output_service)
	{
		printf("Failed to create services\n");
		destroy_io_service(io_service);
		destroy_output_service(output_service);
		return (EXIT_FAILURE);
	}
	env = NULL;
	if (envp)
		env = env_create_from_envp(envp);
	exec_ctx = create_exec_context(&env, io_service, output_service);
	if (!exec_ctx)
	{
		printf("Failed to create execution context\n");
		destroy_io_service(io_service);
		destroy_output_service(output_service);
		return (EXIT_FAILURE);
	}
	exit_code = shell_loop(exec_ctx);
	free_exec_context(exec_ctx);
	if (env)
		env_free(env);
	destroy_io_service(io_service);
	destroy_output_service(output_service);
	return (exit_code);
}
