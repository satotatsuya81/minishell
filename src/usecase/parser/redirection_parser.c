/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/13 23:46:26 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "usecase/parser/parser_internal.h"

/* リダイレクションのパース */
t_cmd_redirect	*parse_redirection(t_token_stream *tokens)
{
	t_redirect_type	type;
	t_cmd_redirect	*redirect;

	if (!tokens->current || tokens->current->type != TOKEN_REDIRECT)
		return (NULL);
	type = tokens->current->value.redirect.redirect_type;
	tokens->current = tokens->current->next;
	if (!tokens->current || tokens->current->type != TOKEN_WORD)
		return (NULL);
	redirect = create_redirect(type, tokens->current->value.word);
	if (!redirect)
		return (NULL);
	tokens->current = tokens->current->next;
	return (redirect);
}
