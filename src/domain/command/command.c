/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 23:17:30 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/13 23:19:18 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "domain/command.h"

void	free_pipeline(t_pipeline *pipeline)
{
	t_pipeline	*next;

	while (pipeline)
	{
		next = pipeline->next;
		free_cmd(pipeline->cmds);
		free(pipeline);
		pipeline = next;
	}
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
	}
	free_redirect(cmd->redirects);
	free_cmd(cmd->next);
	free(cmd);
}

void	free_redirect(t_cmd_redirect *redirect)
{
	t_cmd_redirect	*next;

	while (redirect)
	{
		next = redirect->next;
		free(redirect->file);
		free(redirect);
		redirect = next;
	}
}
