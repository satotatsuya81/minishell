/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:17:02 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:17:31 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft_custom.h"
#include "domain/token.h"
#include "usecase/lexer/token_manager.h"
#include "usecase/lexer/token_creator.h"
#include "usecase/lexer/token_type_handler.h"

/**
 * allways return 1
 * @param c
 */
int	is_word(char c)
{
	return (
		!ft_isspace(c)
		&& !is_single_quote(c)
		&& !is_double_quote(c)
		&& !is_pipe(c)
		&& !is_redirect_input_or_heredoc(c)
		&& !is_redirect_output_or_append(c)
	);
}

int	validate_assignment_first_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	validate_assignment(char *str)
{
	if (*str == '+')
	{
		if (*(str + 1) == '=')
			return (1);
	}
	return (ft_isalnum(*str) || *str == '_');
}

int	handle_word_or_assignment(const char *input,
		t_lexer_state *st, t_token_stream *stream)
{
	int	start;
	int	valid_assignment;
	int	equal_index;

	start = st->index;
	valid_assignment = validate_assignment_first_char(input[st->index]);
	equal_index = -1;
	while (input[st->index] && is_word(input[st->index]))
	{
		if (valid_assignment && input[st->index] == '=' && equal_index == -1)
			equal_index = st->index;
		if (valid_assignment && equal_index == -1)
			valid_assignment = validate_assignment((char *)input + st->index);
		st->index++;
	}
	add_token(stream, create_token(
			TOKEN_WORD, &input[start], st->index - start, st));
	st->column += (st->index - start);
	return (EXIT_SUCCESS);
}
