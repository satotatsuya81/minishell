/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:14:06 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 23:43:23 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft_custom.h"
#include "domain/token.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_creator.h"

int	is_single_quote(char c)
{
	return (c == '\'');
}

int	handle_single_quote(
	const char *input, t_lexer_state *st, t_token_stream *stream)
{
	int		start;
	char	*quoted_content;
	char	*marked_content;

	st->index++;
	st->column++;
	start = st->index;
	while (input[st->index])
	{
		if (input[st->index] == '\n' || input[st->index] == '\'')
			break ;
		st->index++;
		st->column++;
	}
	if (input[st->index] != '\'')
	{
		stream->error_message = ft_strdup("Unclosed single quote");
		stream->error_line = st->line;
		stream->error_column = st->column;
		return (-1);
	}
	quoted_content = ft_strndup(&input[start], st->index - start);
	marked_content = ft_strjoin("\x01", quoted_content);
	free(quoted_content);
	add_token(stream, create_token(TOKEN_WORD, marked_content, 
		ft_strlen(marked_content), st));
	free(marked_content);
	st->index++;
	st->column++;
	return (EXIT_SUCCESS);
}
