/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:03:51 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:37:50 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usecase/env/env_manager.h"
#include "utils/libft_custom.h"
#include "interfaces/io_interface.h"
#include "interfaces/output_interface.h"
#include <stdlib.h>

/**
 * get_path - Get the path to change to.
 * @arg_count: Number of arguments passed to the command.
 * @argv: Array of arguments, where the first argument is the path.
 * @envp: Pointer to the head of the linked list of environment variables.
 *
 * If no arguments are provided,
 * it retrieves the HOME directory from the environment.
 * If an argument is provided, it uses that as the path.
 *
 * Return: The path to change to,
 * or NULL if HOME is not set and no argument is given.
 */
static char	*get_path(char **argv, t_env_var **envp)
{
	char	*home;
	char	*path;

	if (!argv || !*argv)
	{
		home = env_get(*envp, "HOME");
		if (!home)
			return (NULL);
		path = home;
	}
	else
		path = *argv;
	return (path);
}

/**
 * cd with only a relative or absolute path
 * @brief Changes the current working directory to the specified path.
 * If no path is specified, it changes to the HOME directory.
 * @param argv Array of arguments,
 * 	           where the first argument is the path to change to.
 * @param envp Pointer to the head of the linked list of environment variables.
 * @return EXIT_SUCCESS on success,
 *         EXIT_FAILURE on failure
 *         (e.g., if the path does not exist
 *         or is not accessible or too many arguments).
 */
int	ft_cd(char **argv, t_env_var **envp, t_io_service *io,
		t_output_service *out)
{
	char		*path;
	char		*cwd;
	char		*err_msg;
	t_io_result	result;

	if (!io || !out)
		return (EXIT_FAILURE);
	cwd = io->get_current_directory();
	if (!cwd)
		return (EXIT_FAILURE);
	path = get_path(argv, envp);
	if (!path)
	{
		out->write_stderr("bash: cd: HOME not set\n");
		free(cwd);
		return (EXIT_FAILURE);
	}
	result = io->change_directory(path);
	if (result != IO_SUCCESS)
	{
		err_msg = ft_strjoin("bash: cd: ", path);
		if (err_msg)
		{
			out->write_stderr(err_msg);
			out->write_stderr(": ");
			free(err_msg);
		}
		err_msg = io->get_error_message(result);
		if (err_msg)
		{
			out->write_stderr_newline(err_msg);
			free(err_msg);
		}
		free(cwd);
		return (EXIT_FAILURE);
	}
	env_add(envp, "OLDPWD", cwd, "=");
	env_add(envp, "PWD", path, "=");
	free(cwd);
	return (EXIT_SUCCESS);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_env_var	*env = env_create_from_envp(envp);
// 	int			result;

// 	result = ft_cd(argv + 1, &env);
// 	env_free(env);
// 	return (result);
// }
