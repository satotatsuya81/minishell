/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 22:50:01 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 23:46:36 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/env_variable.h"
#include "utils/libft_custom.h"
#include <unistd.h>
#include <stdlib.h>

/**
 * @brief Prints the given strings to stdout, separated by spaces.
 * @param argv The array of strings to print.
 * @return 0 on success, 1 on failure (e.g., if writing to stdout fails).
 */
static int	print_echo(char **argv)
{
	while (*argv)
	{
		if (write(STDOUT_FILENO, *argv, ft_strlen(*argv)) < 0)
			return (EXIT_SUCCESS);
		if (*(argv + 1) && (*argv)[0] != '\0')
			write(STDOUT_FILENO, " ", 1);
		argv++;
	}
	return (EXIT_SUCCESS);
}

/**
 * echo with option -n
 *
 * @brief Prints the given string to stdout.
 * If the string is NULL, it prints an empty line.
 * @param str The string to print.
 * @return 0 on success, 1 on failure (e.g., if writing to stdout fails).
 */
int	ft_echo(char **argv)
{
	int	newline;
	int	result;

	newline = 1;
	result = 0;
	if (!argv || !*argv)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (EXIT_SUCCESS);
	}
	if (ft_strcmp(argv[0], "-n") == 0)
	{
		newline = 0;
		argv++;
	}
	result = print_echo(argv);
	if (result != 0)
		return (EXIT_FAILURE);
	if (newline)
	{
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
