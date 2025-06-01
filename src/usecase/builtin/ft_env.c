/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:57:29 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 09:32:04 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/env_variable.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * env with no options or arguments
 *
 * @brief Prints all environment variables in the format "KEY=VALUE".
 * If a variable has no value, it prints "KEY=".
 * @param envp Pointer to the head of the linked list of environment variables.
 * @return 0 on success, 1 on failure (e.g., if writing to stdout fails).
 */
int	ft_env(t_env_var *envp)
{
	t_env_var	*current;
	int			result_write;

	if (envp == NULL)
		return (EXIT_FAILURE);
	current = envp;
	while (current)
	{
		if (current->value)
			result_write = printf("%s=%s\n", current->key, current->value);
		else
			result_write = printf("%s=\n", current->key);
		if (result_write < 0)
			return (EXIT_FAILURE);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
