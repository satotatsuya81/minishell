/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_context.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/13 23:54:55 by tatsato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "usecase/executor/executor.h"

/* Create execution context */
t_exec_context	*create_exec_context(t_env_var **env,
					t_io_service *io, t_output_service *out)
{
	t_exec_context	*ctx;

	ctx = malloc(sizeof(t_exec_context));
	if (!ctx)
		return (NULL);
	ctx->env = env;
	ctx->io_service = io;
	ctx->output_service = out;
	ctx->last_exit_status = 0;
	ctx->should_exit = 0;
	ctx->exit_code = 0;
	return (ctx);
}

/* Free execution context */
void	free_exec_context(t_exec_context *ctx)
{
	if (ctx)
		free(ctx);
}
