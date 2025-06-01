/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_qupte_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:15:11 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 21:39:52 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft_custom.h"
#include "domain/token.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_creator.h"

int	is_double_quote(char c)
{
	return (c == '\"');
}

int	handle_double_quote(
	const char *input, t_lexer_state *st, t_token_stream *stream)
{
	int	start;

	st->index++;
	st->column++;
	start = st->index;
	while (input[st->index])
	{
		if (input[st->index] == '\n' || input[st->index] == '"')
			break ;
		st->index++;
		st->column++;
	}
	if (input[st->index] != '"')
	{
		stream->error_message = ft_strdup("Unclosed double quote");
		stream->error_line = st->line;
		stream->error_column = st->column;
		return (-1);
	}
	add_token(stream,
		create_token(TOKEN_WORD, &input[start], st->index - start, st));
	st->index++;
	st->column++;
	return (0);
}
