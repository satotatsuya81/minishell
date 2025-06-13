/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/13 23:57:04 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "usecase/executor/executor.h"
#include "usecase/builtin/builtin_commands.h"
#include "utils/libft_custom.h"

/* Check if command is a builtin */
int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/* Execute builtin command */
int	execute_builtin(t_cmd *cmd, t_exec_context *ctx)
{
	char	*command;
	char	**args;
	int		exit_code;

	if (!cmd || !cmd->argv || !cmd->argv[0] || !ctx)
		return (EXIT_FAILURE);
	command = cmd->argv[0];
	args = cmd->argv + 1;
	if (ft_strcmp(command, "echo") == 0)
	{
		ft_echo(args);
		return (EXIT_SUCCESS);
	}
	else if (ft_strcmp(command, "cd") == 0)
		return (ft_cd(args, ctx->env));
	else if (ft_strcmp(command, "pwd") == 0)
	{
		ft_pwd();
		return (EXIT_SUCCESS);
	}
	else if (ft_strcmp(command, "export") == 0)
		return (ft_export(args, ctx->env));
	else if (ft_strcmp(command, "unset") == 0)
		return (ft_unset(args, ctx->env));
	else if (ft_strcmp(command, "env") == 0)
	{
		ft_env(*ctx->env);
		return (EXIT_SUCCESS);
	}
	else if (ft_strcmp(command, "exit") == 0)
	{
		exit_code = 0;
		if (args[0])
		{
			exit_code = ft_atoll(args[0]);
			if (exit_code < 0 || exit_code > 255)
				exit_code = exit_code % 256;
		}
		ctx->should_exit = 1;
		ctx->exit_code = exit_code;
		return (exit_code);
	}
	return (EXIT_FAILURE);
}
