/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:15:07 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/25 22:08:30 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usecase/lexer/token_printer.h"
#include <stdio.h>

void	print_tokens(t_token *token)
{
	while (token)
	{
		if (token->type == TOKEN_WORD)
			printf("WORD: %s\n", token->value.word);
		else if (token->type == TOKEN_PIPE)
			printf("PIPE: %s\n", "|");
		else if (token->type == TOKEN_EOF)
			printf("EOF\n");
		else if (token->type == TOKEN_REDIRECT)
		{
			if (token->value.redirect.redirect_type == REDIRECT_INPUT)
				printf("REDIRECT IN: <\n");
			else if (token->value.redirect.redirect_type == REDIRECT_OUTPUT)
				printf("REDIRECT OUT: >\n");
			else if (token->value.redirect.redirect_type == REDIRECT_HEREDOC)
				printf("HEREDOC: <<\n");
			else if (token->value.redirect.redirect_type == REDIRECT_APPEND)
				printf("APPEND: >>\n");
		}
		else if (token->type == TOKEN_ASSIGNMENT)
			printf("ASSIGNMENT: %s=%s\n",
				token->value.assignment.name, token->value.assignment.value);
		else if (token->type == TOKEN_ERROR)
			printf("ERROR: %s\n", token->value.word);
		else
			printf("UNKNOWN\n");
		token = token->next;
	}
}
