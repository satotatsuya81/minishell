/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_creator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:12:27 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:33:13 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/libft_custom.h"
#include "domain/token.h"
#include "usecase/assignment/assignment_creator.h"

static t_redirect	create_redirect(const char *text, int len)
{
	t_redirect	redirect;

	redirect.redirect_type = REDIRECT_OUTPUT_ERROR;
	if (len == 1)
	{
		if (text[0] == '<')
			redirect.redirect_type = REDIRECT_INPUT;
		else if (text[0] == '>')
			redirect.redirect_type = REDIRECT_OUTPUT;
	}
	else if (len == 2)
	{
		if (text[0] == '<' && text[1] == '<')
			redirect.redirect_type = REDIRECT_HEREDOC;
		else if (text[0] == '>' && text[1] == '>')
			redirect.redirect_type = REDIRECT_APPEND;
	}
	return (redirect);
}

static t_value	create_value(
	t_token_type type, const char *text, int len)
{
	t_value	value;

	value.word = NULL;
	if (type == TOKEN_WORD)
		value.word = ft_strndup(text, len);
	else if (type == TOKEN_ASSIGNMENT)
		value.assignment = create_assignment(text, len);
	else if (type == TOKEN_REDIRECT)
		value.redirect = create_redirect(text, len);
	return (value);
}

t_token	*create_token(
	t_token_type type, const char *text, int len, t_lexer_state *st)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = create_value(type, text, len);
	token->length = len;
	token->line_number = st->line;
	token->column = st->column;
	token->next = NULL;
	return (token);
}
