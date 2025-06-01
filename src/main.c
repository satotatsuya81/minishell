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
#include <readline/readline.h>
#include <readline/history.h>
#include "domain/token.h"
#include "domain/env_initializer.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_printer.h"
#include "usecase/lexer/lexer.h"
#include "usecase/builtin/builtin_commands.h"

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
		stream = lexer(line);
		// ここでトークンを処理する
		// parserはまだ実装できていないので、トークンを表示するだけ
		print_tokens(stream->head);
		// シンプルなコマンド実行でビルトインコマンドなら実行する
		if (stream->head && stream->head->type == TOKEN_WORD)
		{
			char **argv = NULL;
			argv = make_argv(stream->head);
			if (stream->head->value.word
				&& (strcmp(stream->head->value.word, "env") == 0))
				ft_env(env);
			else if (stream->head->value.word
				&& (strcmp(stream->head->value.word, "export") == 0))
				ft_export(argv + 1, &env);
			else if (stream->head->value.word
				&& (strcmp(stream->head->value.word, "unset") == 0))
				ft_unset(argv + 1, &env);
			else if (stream->head->value.word
				&& (strcmp(stream->head->value.word, "exit") == 0))
				ft_exit(argv + 1, stream, env);
			else if (stream->head->value.word
				&& (strcmp(stream->head->value.word, "cd") == 0))
				ft_cd(argv + 1, &env);
			else if (stream->head->value.word
				&& (strcmp(stream->head->value.word, "pwd") == 0))
				ft_pwd();
			else if (stream->head->value.word
				&& (strcmp(stream->head->value.word, "echo") == 0))
				ft_echo(argv + 1);
		}
		free_tokens(stream);
		free(line);
	}
	if (env)
		env_free(env);
	return (0);
}
