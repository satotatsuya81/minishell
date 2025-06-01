/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 07:31:58 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 17:46:15 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "utils/libft_custom.h"
#include "usecase/exit/exit.h"

static int	count_arg(char **argv)
{
	int	count;

	count = 0;
	while (argv && *argv)
	{
		count++;
		argv++;
	}
	return (count);
}

static int	can_convert_str_to_positive_longlong(const char *str)
{
	long long	result;
	int			digit;

	result = 0;
	while (ft_isdigit(*str))
	{
		digit = *str - '0';
		if (result > (LLONG_MAX - digit) / 10)
			return (0);
		result = result * 10 + digit;
		str++;
	}
	if (*str != '\0')
		return (0);
	return (1);
}

static int	can_convert_str_to_negative_longlong(const char *str)
{
	long long	result;
	int			digit;

	result = 0;
	while (ft_isdigit(*str))
	{
		digit = *str - '0';
		if (result < (LLONG_MIN + digit) / 10)
			return (0);
		result = result * 10 - digit;
		str++;
	}
	if (*str != '\0')
		return (0);
	return (1);
}

static int	can_convert_str_to_longlong(const char *str)
{
	if (*str == '-')
	{
		str++;
		return (can_convert_str_to_negative_longlong(str));
	}
	else if (*str == '+')
	{
		str++;
		return (can_convert_str_to_positive_longlong(str));
	}
	else
		return (can_convert_str_to_positive_longlong(str));
}

void	ft_exit(char **argv, t_token_stream *stream, t_env_var *env)
{
	long long	exit_code;
	int			argc;

	write(STDERR_FILENO, "exit\n", 5);
	if (!argv || !*argv)
		exit_minishell(0, stream, env);
	argc = count_arg(argv);
	if (argc > 1)
	{
		write(STDERR_FILENO, "bash: exit: too many arguments\n", 31);
		exit_minishell(1, stream, env);
	}
	if (can_convert_str_to_longlong(*argv) == 0)
	{
		write(STDERR_FILENO, "bash: exit: numeric argument required\n", 38);
		exit_minishell(255, stream, env);
	}
	exit_code = my_atoll(*argv);
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	if (exit_code < 0)
		exit_code = 0;
	if (exit_code > 255)
		exit_code = 255;
	exit_minishell(exit_code, stream, env);
}

// int main(int argc, char **argv)
// {
// 	argv++;
// 	ft_exit(argv);
// }
