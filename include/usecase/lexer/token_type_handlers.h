/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_handlers.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:23:48 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/18 23:48:10 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_TYPE_HANDLERS_H
# define TOKEN_TYPE_HANDLERS_H
# include <usecase/lexer/token_type_handler.h>
# include <usecase/lexer/lexer.h>

t_token_type_handlers	create_token_handlers(void);
void					free_token_handlers(t_token_type_handlers *handlers);
int						dispatch_token_handler(const char *input,
							t_lexer_state *st, t_token_stream *stream,
							t_token_type_handlers *handlers);

#endif
