/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_service.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/16 08:33:51 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include "interfaces/io_interface.h"
#include "utils/libft_custom.h"

/* No global variables - using dependency injection */

static t_io_result	impl_change_directory(const char *path)
{
	if (!path)
		return (IO_ERROR_ACCESS);
	if (chdir(path) == -1)
	{
		if (errno == EACCES)
			return (IO_ERROR_PERMISSION);
		else if (errno == ENOENT)
			return (IO_ERROR_NOT_FOUND);
		else
			return (IO_ERROR_SYSTEM);
	}
	return (IO_SUCCESS);
}

static char	*impl_get_current_directory(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	return (cwd);
}

static bool	impl_file_exists(const char *path)
{
	struct stat	st;

	if (!path)
		return (false);
	return (stat(path, &st) == 0);
}

static bool	impl_is_directory(const char *path)
{
	struct stat	st;

	if (!path || stat(path, &st) == -1)
		return (false);
	return (S_ISDIR(st.st_mode));
}

static t_io_result	impl_create_directory(const char *path)
{
	if (!path)
		return (IO_ERROR_ACCESS);
	if (mkdir(path, 0755) == -1)
	{
		if (errno == EACCES)
			return (IO_ERROR_PERMISSION);
		else if (errno == EEXIST)
			return (IO_SUCCESS);
		else
			return (IO_ERROR_SYSTEM);
	}
	return (IO_SUCCESS);
}

static char	*impl_get_error_message(t_io_result result)
{
	if (result == IO_ERROR_ACCESS)
		return (ft_strdup("Access denied"));
	else if (result == IO_ERROR_NOT_FOUND)
		return (ft_strdup("No such file or directory"));
	else if (result == IO_ERROR_PERMISSION)
		return (ft_strdup("Permission denied"));
	else if (result == IO_ERROR_SYSTEM)
		return (ft_strdup("System error"));
	return (ft_strdup("Unknown error"));
}

t_io_service	*create_io_service(void)
{
	t_io_service	*service;

	service = malloc(sizeof(t_io_service));
	if (!service)
		return (NULL);
	service->change_directory = impl_change_directory;
	service->get_current_directory = impl_get_current_directory;
	service->file_exists = impl_file_exists;
	service->is_directory = impl_is_directory;
	service->create_directory = impl_create_directory;
	service->get_error_message = impl_get_error_message;
	return (service);
}

void	destroy_io_service(t_io_service *service)
{
	if (service)
		free(service);
}
