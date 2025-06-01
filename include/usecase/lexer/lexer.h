/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:09:31 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 14:43:28 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "domain/token.h"

t_token_stream	*lexer(const char *input);
void			free_tokens(t_token_stream *stream);

#endif
