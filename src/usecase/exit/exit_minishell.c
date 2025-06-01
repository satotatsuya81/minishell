/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:42:06 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 17:49:18 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "domain/token.h"
#include "domain/env_variable.h"
#include "domain/env_initializer.h"
#include "usecase/lexer/token_manager.h"

void	exit_minishell(int exit_code, t_token_stream *stream, t_env_var *env)
{
	if (stream)
		free_tokens(stream);
	if (env)
		env_free(env);
	exit(exit_code);
}
