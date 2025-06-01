/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_manager.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:06:49 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 21:39:28 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_MANAGER_H
# define TOKEN_MANAGER_H

# include "domain/token.h"

void	add_token(t_token_stream *stream, t_token *token);
void	free_tokens(t_token_stream *stream);

#endif
