/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:27:44 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 23:49:34 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "domain/env_variable.h"
#include "utils/libft_custom.h"
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>

int	ft_pwd(void)
{
	char	cwd[MAXPATHLEN];

	if (getcwd(cwd, MAXPATHLEN))
	{
		if (write(STDOUT_FILENO, cwd, ft_strlen(cwd)) < 0)
			return (EXIT_FAILURE);
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	else
		return (EXIT_FAILURE);
}
