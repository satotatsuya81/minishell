/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_output.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/13 23:19:59 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_OUTPUT_H
# define DEBUG_OUTPUT_H

# include "domain/token.h"

void	print_lexer_summary(t_token_stream *stream);

#endif