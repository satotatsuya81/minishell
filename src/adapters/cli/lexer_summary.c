/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_summary.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/13 23:40:53 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "adapters/cli/debug_output.h"
#include "domain/token.h"

static int	count_total_tokens(t_token_stream *stream)
{
	int		token_count;
	t_token	*current;

	token_count = 0;
	current = stream->head;
	while (current && current->type != TOKEN_EOF)
	{
		token_count++;
		current = current->next;
	}
	return (token_count);
}

static void	print_error_info(t_token_stream *stream, int token_count)
{
	printf("❌ Lexer error: %s (line %d, column %d)\n",
		stream->error_message, stream->error_line,
		stream->error_column);
	printf("📊 Tokens before error: %d\n", token_count);
}

void	print_lexer_summary(t_token_stream *stream)
{
	int	token_count;

	printf("=== Lexer Results ===\n");
	token_count = count_total_tokens(stream);
	if (stream->error_message)
		print_error_info(stream, token_count);
	else
		print_success_info(stream, token_count);
	printf("\n");
}
