/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_creator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 15:59:46 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 17:03:32 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/token.h"
#include "utils/libft_custom.h"

static t_assignment	init_assignment(void)
{
	t_assignment	assignment;

	assignment.name = NULL;
	assignment.value = NULL;
	assignment.operator = NULL;
	assignment.text = NULL;
	return (assignment);
}

t_assignment	create_assignment(const char *text, int len)
{
	t_assignment	assignment;
	int				equal_index;

	equal_index = 0;
	assignment = init_assignment();
	while (text[equal_index] && text[equal_index] != '=' && equal_index < len)
		equal_index++;
	if (equal_index == len || equal_index == 0)
		return (assignment);
	if (text[equal_index - 1] == '+')
	{
		assignment.name = ft_strndup(text, equal_index - 1);
		assignment.operator = ft_strndup("+=", 2);
	}
	else
	{
		assignment.name = ft_strndup(text, equal_index);
		assignment.operator = ft_strndup("=", 1);
	}
	assignment.value
		= ft_strndup(text + equal_index + 1, len - equal_index - 1);
	assignment.text = ft_strndup(text, len);
	return (assignment);
}

void	free_assignment(t_assignment *assignment)
{
	if (assignment->name)
		free(assignment->name);
	if (assignment->value)
		free(assignment->value);
	if (assignment->operator)
		free(assignment->operator);
	if (assignment->text)
		free(assignment->text);
}
