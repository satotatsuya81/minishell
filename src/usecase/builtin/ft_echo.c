/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 22:50:01 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:36:37 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interfaces/output_interface.h"
#include "utils/libft_custom.h"
#include <stdlib.h>

/**
 * @brief Prints the given strings to stdout, separated by spaces.
 * @param argv The array of strings to print.
 * @return 0 on success, 1 on failure (e.g., if writing to stdout fails).
 */
static int	print_echo(char **argv, t_output_service *out)
{
	while (*argv)
	{
		if (out->write_stdout(*argv) != OUTPUT_SUCCESS)
			return (EXIT_FAILURE);
		if (*(argv + 1) && (*argv)[0] != '\0')
			out->write_stdout(" ");
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
int	ft_echo(char **argv, t_output_service *out)
{
	int	newline;
	int	result;

	if (!out)
		return (EXIT_FAILURE);
	newline = 1;
	if (!argv || !*argv)
	{
		out->write_stdout("\n");
		return (EXIT_SUCCESS);
	}
	if (ft_strcmp(argv[0], "-n") == 0)
	{
		newline = 0;
		argv++;
	}
	result = print_echo(argv, out);
	if (result != 0)
		return (EXIT_FAILURE);
	if (newline)
	{
		if (out->write_stdout("\n") != OUTPUT_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
