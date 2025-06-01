/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:48:08 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/25 18:29:15 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <usecase/lexer/token_type_handler.h>
#include <stdlib.h>

t_token_type_handlers	create_token_handlers(void)
{
	t_token_type_handlers	handlers;

	handlers.token_handler_count = 7;
	handlers.token_handlers
		= malloc(sizeof(t_token_handler) * handlers.token_handler_count);
	if (!handlers.token_handlers)
	{
		handlers.token_handler_count = 0;
		return (handlers);
	}
	handlers.token_handlers[0] = (t_token_handler){ft_isspace,
	{.handler_regular = handle_space}};
	handlers.token_handlers[1] = (t_token_handler){is_single_quote,
	{.handler_regular = handle_single_quote}};
	handlers.token_handlers[2] = (t_token_handler){is_double_quote,
	{.handler_regular = handle_double_quote}};
	handlers.token_handlers[3] = (t_token_handler){is_pipe,
	{.handler_regular = handle_pipe}};
	handlers.token_handlers[4] = (t_token_handler){is_redirect_input_or_heredoc,
	{.handler_regular = handle_redirect_input_or_heredoc}};
	handlers.token_handlers[5] = (t_token_handler){is_redirect_output_or_append,
	{.handler_regular = handle_redirect_output_or_append}};
	handlers.token_handlers[6] = (t_token_handler){is_word,
	{.handler_regular = handle_word_or_assignment}};
	return (handlers);
}

void	free_token_handlers(t_token_type_handlers *handlers)
{
	if (handlers->token_handlers)
	{
		free(handlers->token_handlers);
		handlers->token_handlers = NULL;
	}
}

int	dispatch_token_handler(const char *input,
		t_lexer_state *st, t_token_stream *stream,
		t_token_type_handlers *handlers)
{
	int	i;

	i = 0;
	while (i < handlers->token_handler_count)
	{
		if (handlers->token_handlers[i].match(input[st->index]))
			return (handlers->token_handlers[i].handler.handler_regular(input,
					st, stream));
		i++;
	}
	return (-1);
}
