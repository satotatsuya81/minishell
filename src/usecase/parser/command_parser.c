/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirokugo <mirokugo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/13 23:19:59 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/usecase/parser/parser_internal.h"
#include "../../../src/utils/libft/libft.h"

int	add_word_to_cmd(t_cmd *cmd, t_token_stream *tokens, int *argc)
{
	cmd->argv = extend_argv(cmd->argv, *argc);
	if (!cmd->argv)
		return (0);
	cmd->argv[*argc] = ft_strdup(tokens->current->value.word);
	if (!cmd->argv[*argc])
		return (0);
	(*argc)++;
	tokens->current = tokens->current->next;
	return (1);
}

int	add_redirect_to_cmd(t_cmd *cmd, t_token_stream *tokens,
		t_cmd_redirect **last_redirect)
{
	t_cmd_redirect	*new_redirect;

	new_redirect = parse_redirection(tokens);
	if (!new_redirect)
		return (0);
	if (!cmd->redirects)
	{
		cmd->redirects = new_redirect;
		*last_redirect = new_redirect;
	}
	else
	{
		(*last_redirect)->next = new_redirect;
		*last_redirect = new_redirect;
	}
	return (1);
}

t_cmd	*init_simple_command(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	process_command_tokens(t_cmd *cmd, t_token_stream *tokens,
		int *argc, t_cmd_redirect **last_redirect)
{
	while (tokens->current && tokens->current->type != TOKEN_PIPE
		&& tokens->current->type != TOKEN_EOF)
	{
		if (tokens->current->type == TOKEN_WORD)
		{
			if (!add_word_to_cmd(cmd, tokens, argc))
				return (0);
		}
		else if (tokens->current->type == TOKEN_REDIRECT)
		{
			if (!add_redirect_to_cmd(cmd, tokens, last_redirect))
				return (0);
		}
		else
			break ;
	}
	return (1);
}

t_cmd	*parse_simple_command(t_token_stream *tokens)
{
	t_cmd			*cmd;
	t_cmd_redirect	*last_redirect;
	int				argc;

	cmd = init_simple_command();
	if (!cmd)
		return (NULL);
	last_redirect = NULL;
	argc = 0;
	if (!process_command_tokens(cmd, tokens, &argc, &last_redirect))
		return (NULL);
	if (argc > 0)
		cmd->argv[argc] = NULL;
	return (cmd);
}
