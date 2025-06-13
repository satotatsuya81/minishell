/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_factory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirokugo <mirokugo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/13 23:19:59 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/usecase/parser/parser_internal.h"
#include "../../../src/utils/libft/libft.h"

t_parse_result	*create_parse_result(void)
{
	t_parse_result	*result;

	result = malloc(sizeof(t_parse_result));
	if (!result)
		return (NULL);
	result->ast = NULL;
	result->error_msg = NULL;
	result->error_line = 0;
	result->error_column = 0;
	return (result);
}

t_pipeline	*create_pipeline(void)
{
	t_pipeline	*pipeline;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	pipeline->cmds = NULL;
	pipeline->next = NULL;
	pipeline->connector = CONN_NONE;
	return (pipeline);
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	init_redirect_file(t_cmd_redirect *redirect, const char *file)
{
	redirect->file = ft_strdup(file);
	if (!redirect->file)
	{
		free(redirect);
		return (0);
	}
	return (1);
}

t_cmd_redirect	*create_redirect(t_redirect_type type, const char *file)
{
	t_cmd_redirect	*redirect;

	redirect = malloc(sizeof(t_cmd_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	if (!init_redirect_file(redirect, file))
		return (NULL);
	redirect->fd = -1;
	redirect->next = NULL;
	return (redirect);
}
