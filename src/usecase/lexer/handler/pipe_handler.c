/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:15:51 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 21:40:02 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/token.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_creator.h"

int	is_pipe(char c)
{
	return (c == '|');
}

int	handle_pipe(const char *input,
		t_lexer_state *st, t_token_stream *stream)
{
	add_token(stream,
		create_token(TOKEN_PIPE, &input[st->index], 1, st));
	st->index++;
	st->column++;
	return (0);
}
