/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_output.c                                     :+:      :+:    :+:   */
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
#include "usecase/parser/parser_interface.h"

static void	count_token_types(t_token_stream *stream, int *counts)
{
	t_token	*current;

	current = stream->head;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
			counts[0]++;
		else if (current->type == TOKEN_PIPE)
			counts[1]++;
		else if (current->type == TOKEN_REDIRECT)
			counts[2]++;
		else if (current->type == TOKEN_ASSIGNMENT)
			counts[3]++;
		current = current->next;
	}
}

static void	print_token_counts(int *counts)
{
	printf("   - WORD tokens: %d\n", counts[0]);
	if (counts[1] > 0)
		printf("   - PIPE tokens: %d\n", counts[1]);
	if (counts[2] > 0)
		printf("   - REDIRECT tokens: %d\n", counts[2]);
	if (counts[3] > 0)
		printf("   - ASSIGNMENT tokens: %d\n", counts[3]);
}

void	print_lexer_summary(t_token_stream *stream)
{
	int		token_count;
	int		counts[4];
	t_token	*current;

	token_count = 0;
	counts[0] = 0;
	counts[1] = 0;
	counts[2] = 0;
	counts[3] = 0;
	current = stream->head;
	printf("=== Lexer Results ===\n");
	while (current && current->type != TOKEN_EOF)
	{
		token_count++;
		current = current->next;
	}
	if (stream->error_message)
	{
		printf("❌ Lexer error: %s (line %d, column %d)\n",
			stream->error_message, stream->error_line,
			stream->error_column);
		printf("📊 Tokens before error: %d\n", token_count);
	}
	else
	{
		printf("✅ Lexer successful: %d tokens generated\n", token_count);
		printf("📊 Token breakdown:\n");
		count_token_types(stream, counts);
		print_token_counts(counts);
	}
	printf("\n");
}

