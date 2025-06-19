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
#include "adapters/parser/parser_interface.h"

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
	(void)counts;
}

void	print_success_info(t_token_stream *stream, int token_count)
{
	int	counts[4];

	counts[0] = 0;
	counts[1] = 0;
	counts[2] = 0;
	counts[3] = 0;
	count_token_types(stream, counts);
	print_token_counts(counts);
	(void)token_count;
}
