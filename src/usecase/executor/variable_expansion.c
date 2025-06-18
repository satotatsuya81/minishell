/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/18 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "usecase/executor/executor.h"
#include "usecase/env/env_manager.h"
#include "utils/libft_custom.h"

static char	*get_variable_value(const char *var_name, t_exec_context *ctx)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
	{
		return (ft_itoa(ctx->last_exit_status));
	}
	value = env_get(*ctx->env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static char	*extract_variable_name(const char *str, int *end_pos)
{
	int		start;
	int		end;
	char	*var_name;

	start = 1;
	end = start;
	if (str[start] == '?')
	{
		*end_pos = start + 1;
		return (ft_strdup("?"));
	}
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	*end_pos = end;
	if (end == start)
		return (NULL);
	var_name = malloc(end - start + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, str + start, end - start + 1);
	return (var_name);
}

static char	*expand_single_variable(const char *str, t_exec_context *ctx)
{
	char	*var_name;
	char	*var_value;
	char	*before_var;
	char	*after_var;
	char	*result;
	char	*temp;
	int		dollar_pos;
	int		var_end_pos;

	dollar_pos = 0;
	while (str[dollar_pos] && str[dollar_pos] != '$')
		dollar_pos++;
	if (!str[dollar_pos])
		return (ft_strdup(str));
	var_name = extract_variable_name(str + dollar_pos, &var_end_pos);
	if (!var_name)
		return (ft_strdup(str));
	var_value = get_variable_value(var_name, ctx);
	before_var = ft_substr(str, 0, dollar_pos);
	after_var = ft_strdup(str + dollar_pos + var_end_pos);
	temp = ft_strjoin(before_var, var_value);
	result = ft_strjoin(temp, after_var);
	free(var_name);
	free(var_value);
	free(before_var);
	free(after_var);
	free(temp);
	return (result);
}

char	*expand_variables(const char *str, t_exec_context *ctx)
{
	char	*result;
	char	*temp;
	int		has_dollar;

	if (!str || !ctx)
		return (NULL);
	result = ft_strdup(str);
	if (!result)
		return (NULL);
	has_dollar = 1;
	while (has_dollar)
	{
		temp = result;
		result = expand_single_variable(temp, ctx);
		has_dollar = (ft_strchr(result, '$') != NULL);
		if (ft_strcmp(temp, result) == 0)
			has_dollar = 0;
		free(temp);
		if (!result)
			return (NULL);
	}
	return (result);
}

void	expand_command_variables(t_cmd *cmd, t_exec_context *ctx)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->argv || !ctx)
		return ;
	i = 0;
	while (cmd->argv[i])
	{
		if (cmd->argv[i][0] == '\x01')
		{
			expanded = ft_strdup(cmd->argv[i] + 1);
			free(cmd->argv[i]);
			cmd->argv[i] = expanded;
		}
		else
		{
			expanded = expand_variables(cmd->argv[i], ctx);
			if (expanded)
			{
				free(cmd->argv[i]);
				cmd->argv[i] = expanded;
			}
		}
		i++;
	}
}