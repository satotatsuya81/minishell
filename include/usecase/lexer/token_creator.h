/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_creator.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:14:29 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:04:03 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_CREATOR_H
# define TOKEN_CREATOR_H

# include "domain/token.h"

t_token	*create_token(t_token_type type,
			const char *text, int len, t_lexer_state *st);
#endif
