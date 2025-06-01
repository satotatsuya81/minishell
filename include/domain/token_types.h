/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:09:07 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 18:09:44 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_TYPES_H
# define TOKEN_TYPES_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_ASSIGNMENT,
	TOKEN_OPERATOR,
	TOKEN_REDIRECT,
	TOKEN_PIPE,
	TOKEN_EOF,
	TOKEN_ERROR
}	t_token_type;

/* リダイレクトのタイプ */
typedef enum e_redirect_type
{
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
	REDIRECT_OUTPUT_ERROR
}	t_redirect_type;

#endif
