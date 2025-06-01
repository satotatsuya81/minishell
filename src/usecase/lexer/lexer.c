/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_service.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:19:37 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 21:40:56 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "utils/libft_custom.h"
#include "domain/token.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_creator.h"
#include "usecase/lexer/token_type_handler.h"
#include "usecase/lexer/token_type_handlers.h"
#include "usecase/lexer/token_printer.h"

static t_token_stream	*create_token_stream(void)
{
	t_token_stream	*stream;

	stream = malloc(sizeof(t_token_stream));
	if (!stream)
		return (NULL);
	ft_memset(stream, 0, sizeof(t_token_stream));
	return (stream);
}

t_token_stream	*lexer(const char *input)
{
	t_token_stream			*stream;
	t_lexer_state			st;
	t_token_type_handlers	handlers;

	stream = create_token_stream();
	if (!stream)
		return (NULL);
	handlers = create_token_handlers();
	if (!handlers.token_handler_count)
	{
		free(stream);
		return (NULL);
	}
	st = (t_lexer_state){0, 1, 1};
	while (input[st.index])
	{
		if (dispatch_token_handler(input, &st, stream, &handlers) == -1)
		{
			free_token_handlers(&handlers);
			return (stream);
		}
	}
	add_token(stream, create_token(TOKEN_EOF, "", 0, &st));
	free_token_handlers(&handlers);
	return (stream);
}
