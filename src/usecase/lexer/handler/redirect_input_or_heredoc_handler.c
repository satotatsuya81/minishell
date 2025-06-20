/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_input_or_heredoc_handler.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:17:02 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 21:40:12 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft_custom.h"
#include "domain/token.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_creator.h"
#include "usecase/lexer/token_type_handler.h"
/**
 * allways return 1
 * @param c
 */
int	is_redirect_input_or_heredoc(char c)
{
	return (c == '<');
}

int	handle_redirect_input_or_heredoc(const char *input,
		t_lexer_state *st, t_token_stream *stream)
{
	int				start;

	start = st->index;
	st->index++;
	if (input[st->index] == '<')
		st->index++;
	add_token(stream, create_token(TOKEN_REDIRECT, input + start,
			st->index - start, st));
	return (1);
}
