/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:44:11 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 17:45:53 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H
# include "domain/token.h"
# include "domain/env_variable.h"

void	exit_minishell(int exit_code, t_token_stream *stream, t_env_var *env);

#endif
