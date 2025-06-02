/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:17:10 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 17:46:52 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "domain/token.h"
#include "domain/env_initializer.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_printer.h"
#include "usecase/lexer/lexer.h"
#include "usecase/builtin/builtin_commands.h"
#include "usecase/parser/parser_interface.h"
#include "domain/command.h"

static char	**make_argv(t_token *token)
{
	char	**argv;
	int		count;
	t_token	*current;

	if (!token)
		return (NULL);
	count = 0;
	current = token;
	while (current)
	{
		if (current->type == TOKEN_WORD)
			count++;
		if (current->type == TOKEN_ASSIGNMENT)
			count++;
		current = current->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	count = 0;
	current = token;
	while (current)
	{
		if (current->type == TOKEN_WORD)
			argv[count++] = current->value.word;
		if (current->type == TOKEN_ASSIGNMENT)
		{
			// ビルトインコマンドの引数としては無視する
			argv[count++] = current->value.assignment.text;
		}
		current = current->next;
	}
	argv[count] = NULL;
	return (argv);
}

static void	print_parse_result(t_parse_result *result)
{
	if (!result) {
		printf("❌ Parse failed: result is NULL\n");
		return;
	}
	
	if (result->error_msg) {
		printf("❌ Parse error: %s (line %d, column %d)\n", 
			result->error_msg, result->error_line, result->error_column);
		return;
	}
	
	if (!result->ast) {
		printf("❌ Parse failed: AST is NULL\n");
		return;
	}
	
	printf("✅ Parse successful! AST created.\n");
}

static void	execute_simple_command(t_cmd *cmd, t_env_var **env)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return;
	
	// ビルトインコマンドの実行
	if (strcmp(cmd->argv[0], "env") == 0)
		ft_env(*env);
	else if (strcmp(cmd->argv[0], "export") == 0)
		ft_export(cmd->argv + 1, env);
	else if (strcmp(cmd->argv[0], "unset") == 0)
		ft_unset(cmd->argv + 1, env);
	else if (strcmp(cmd->argv[0], "cd") == 0)
		ft_cd(cmd->argv + 1, env);
	else if (strcmp(cmd->argv[0], "pwd") == 0)
		ft_pwd();
	else if (strcmp(cmd->argv[0], "echo") == 0)
		ft_echo(cmd->argv + 1);
	else if (strcmp(cmd->argv[0], "exit") == 0)
	{
		// exitは特殊処理が必要なので一旦スキップ
		printf("exit command detected\n");
	}
	else
	{
		printf("Command not found: %s\n", cmd->argv[0]);
	}
}

static void	execute_pipeline(t_pipeline *pipeline, t_env_var **env)
{
	if (!pipeline)
		return;
	
	// 現在は単純なコマンドのみ対応
	if (pipeline->cmds && !pipeline->cmds->next && !pipeline->cmds->redirects)
	{
		execute_simple_command(pipeline->cmds, env);
	}
	else
	{
		printf("Complex commands (pipes, redirections) not yet implemented\n");
	}
	
	// 次のパイプライン（&&, ||, ;）は未実装
	if (pipeline->next)
	{
		printf("Operators (&&, ||, ;) not yet implemented\n");
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	t_token_stream	*stream;
	t_env_var		*env;

	(void)argc;
	(void)argv;
	if (envp)
		env = env_create_from_envp(envp);
	while (1)
	{
		line = readline("minishell> ");
		if (!line) {
			// Ctrl-DなどでNULLが返る → 終了
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		
		// レキサーでトークン化
		stream = lexer(line);
		
		// デバッグ用：トークンを表示
		printf("=== Tokens ===\n");
		print_tokens(stream->head);
		
		// パーサーでAST作成
		t_parse_result *result = parse(stream);
		
		// パース結果を表示
		print_parse_result(result);
		
		// パース成功時は実行
		if (result && !result->error_msg && result->ast)
		{
			execute_pipeline(result->ast, &env);
		}
		
		// exitコマンドの特殊処理
		if (stream->head && stream->head->type == TOKEN_WORD 
			&& strcmp(stream->head->value.word, "exit") == 0)
		{
			char **argv = make_argv(stream->head);
			ft_exit(argv + 1, stream, env);
		}
		
		// メモリ解放
		if (result)
			free_parse_result(result);
		free_tokens(stream);
		free(line);
	}
	if (env)
		env_free(env);
	return (0);
}
