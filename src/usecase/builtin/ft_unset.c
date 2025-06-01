/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:50:27 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 23:43:14 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/env_variable.h"
#include <stdlib.h>

/**
 * unset with no options
 * @brief Removes environment variables from the linked list.
 * It iterates through the provided arguments, and for each argument,
 * it calls `env_remove` to remove the corresponding variable from the list.
 * @param argv
 *  Array of strings representing the keys of the environment variables to unset.
 * @param envp Pointer to the head of the linked list of environment variables.
 * @return 0 on success, as the function does not return any error codes.
 */
int	ft_unset(char **argv, t_env_var **envp)
{
	char	*key;

	if (!argv || !*argv)
		return (EXIT_SUCCESS);
	while (*argv)
	{
		key = *argv++;
		if (key && *key)
			env_remove(envp, key);
	}
	return (EXIT_SUCCESS);
}
