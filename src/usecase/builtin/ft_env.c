/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:57:29 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:38:12 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usecase/env/env_manager.h"
#include "interfaces/output_interface.h"
#include "utils/libft_custom.h"
#include <stdlib.h>

/**
 * env with no options or arguments
 *
 * @brief Prints all environment variables in the format "KEY=VALUE".
 * If a variable has no value, it prints "KEY=".
 * @param envp Pointer to the head of the linked list of environment variables.
 * @return 0 on success, 1 on failure (e.g., if writing to stdout fails).
 */
int	ft_env(t_env_var *envp, t_output_service *out)
{
	t_env_var	*current;
	char		*output;
	char		*temp;

	if (!envp || !out)
		return (EXIT_FAILURE);
	current = envp;
	while (current)
	{
		output = ft_strjoin(current->key, "=");
		if (!output)
			return (EXIT_FAILURE);
		if (current->value)
		{
			temp = ft_strjoin(output, current->value);
			free(output);
			output = temp;
			if (!output)
				return (EXIT_FAILURE);
		}
		if (out->write_stdout_newline(output) != OUTPUT_SUCCESS)
		{
			free(output);
			return (EXIT_FAILURE);
		}
		free(output);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
