/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:19:37 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:56:57 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "domain/token.h"
#include "usecase/assignment/assignment_creator.h"
#include "usecase/lexer/token_creator.h"

void	add_token(t_token_stream *stream, t_token *token)
{
	if (!stream->head)
	{
		stream->head = token;
		stream->current = token;
	}
	else
	{
		stream->current->next = token;
		stream->current = token;
	}
	stream->count++;
}

void	free_tokens(t_token_stream *stream)
{
	t_token	*cur;
	t_token	*next;

	cur = stream->head;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOKEN_WORD)
			free(cur->value.word);
		else if (cur->type == TOKEN_ASSIGNMENT)
			free_assignment(&cur->value.assignment);
		free(cur);
		cur = next;
	}
	free(stream);
}
