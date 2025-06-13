/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/13 23:40:14 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "adapters/cli/output_utils.h"
#include "domain/token.h"
#include "domain/command.h"
#include "usecase/parser/parser_interface.h"
#include "usecase/executor/executor.h"

void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("\t");
		i++;
	}
}

void	print_pipeline_cmds(t_pipeline *ast, int depth)
{
	int	j;

	if (ast->cmds)
	{
		print_indent(depth);
		printf("Cmds:\n");
		j = 0;
		while (ast->cmds->argv && ast->cmds->argv[j])
		{
			print_indent(depth + 1);
			printf("argv[%d]: %s\n", j, ast->cmds->argv[j]);
			j++;
		}
		if (j == 0)
		{
			print_indent(depth + 1);
			printf("No arguments.\n");
		}
	}
	else
	{
		print_indent(depth);
		printf("No cmds.\n");
	}
}

void	print_ast_debug(t_pipeline *ast, int depth)
{
	print_indent(depth);
	if (!ast)
	{
		printf("NULL\n");
		return ;
	}
	printf("Pipeline at %p\n", ast);
	print_pipeline_cmds(ast, depth);
	if (ast->next)
	{
		print_indent(depth);
		printf("Next pipeline:\n");
		print_ast_debug(ast->next, depth + 1);
	}
}

void	print_ast(t_pipeline *ast)
{
	printf("AST Debug:\n");
	print_ast_debug(ast, 1);
}

void	print_execution_summary(int status, t_exec_context *ctx)
{
	if (ctx->should_exit)
		printf("✅ Exit command executed (code: %d)\n", ctx->exit_code);
	else if (status == EXIT_SUCCESS)
		printf("✅ Command executed successfully\n");
	else if (status == EXIT_COMMAND_NOT_FOUND)
		printf("❌ Command not found\n");
	else
		printf("❌ Command failed with exit code: %d\n", status);
}

