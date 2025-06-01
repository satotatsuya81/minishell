/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_handler.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:07:59 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 14:43:01 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_TYPE_HANDLER_H
# define TOKEN_TYPE_HANDLER_H

# include <domain/token.h>

typedef union u_token_handle_function
{
	int	(*handler_regular)(const char *input,
			t_lexer_state *st, t_token_stream *stream);
	int	(*handler_simple)(t_lexer_state *st);
}	t_token_handle_function;

typedef struct s_token_handler
{
	// トークンのタイプ
	int						(*match)(const char input);
	// トークンのハンドラ
	t_token_handle_function	handler;
}	t_token_handler;

typedef struct s_token_type_handlers
{
	// トークンのハンドラの配列
	t_token_handler	*token_handlers;
	// トークンのハンドラの数
	int				token_handler_count;
}	t_token_type_handlers;

int		ft_isspace(const char c);
int		is_single_quote(char c);
int		is_double_quote(char c);
int		is_pipe(char c);
int		is_word(char c);
int		is_redirect_input_or_heredoc(char c);
int		is_redirect_output_or_append(char c);
int		handle_space(const char *input,
			t_lexer_state *st, t_token_stream *stream);
int		handle_single_quote(const char *input,
			t_lexer_state *st, t_token_stream *stream);
int		handle_double_quote(const char *input,
			t_lexer_state *st, t_token_stream *stream);
int		handle_pipe(const char *input,
			t_lexer_state *st, t_token_stream *stream);
int		handle_word_or_assignment(const char *input,
			t_lexer_state *st, t_token_stream *stream);
int		handle_redirect_input_or_heredoc(const char *input,
			t_lexer_state *st, t_token_stream *stream);
int		handle_redirect_output_or_append(const char *input,
			t_lexer_state *st, t_token_stream *stream);

#endif
