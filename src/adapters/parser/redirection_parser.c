/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirokugo <mirokugo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/01 21:45:00 by mirokugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/interfaces/parser_internal.h"

/* リダイレクションのパース */
t_cmd_redirect *parse_redirection(t_token_stream *tokens)
{
    if (!tokens->current || tokens->current->type != TOKEN_REDIRECT)
        return NULL;
    
    t_redirect_type type = tokens->current->value.redirect.redirect_type;
    tokens->current = tokens->current->next;
    
    if (!tokens->current || tokens->current->type != TOKEN_WORD)
        return NULL;
    
    t_cmd_redirect *redirect = create_redirect(type, tokens->current->value.word);
    if (!redirect)
        return NULL;
    
    tokens->current = tokens->current->next;
    return redirect;
}