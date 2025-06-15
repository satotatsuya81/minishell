/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_service.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/16 08:34:06 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "interfaces/output_interface.h"
#include "utils/libft_custom.h"

/* No global variables - using dependency injection */

static t_output_result	impl_write_stdout(const char *message)
{
	if (!message)
		return (OUTPUT_ERROR_WRITE);
	if (write(STDOUT_FILENO, message, ft_strlen(message)) == -1)
		return (OUTPUT_ERROR_SYSTEM);
	return (OUTPUT_SUCCESS);
}

static t_output_result	impl_write_stderr(const char *message)
{
	if (!message)
		return (OUTPUT_ERROR_WRITE);
	if (write(STDERR_FILENO, message, ft_strlen(message)) == -1)
		return (OUTPUT_ERROR_SYSTEM);
	return (OUTPUT_SUCCESS);
}

static t_output_result	impl_write_stdout_newline(const char *message)
{
	t_output_result	result;

	result = impl_write_stdout(message);
	if (result != OUTPUT_SUCCESS)
		return (result);
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return (OUTPUT_ERROR_SYSTEM);
	return (OUTPUT_SUCCESS);
}

static t_output_result	impl_write_stderr_newline(const char *message)
{
	t_output_result	result;

	result = impl_write_stderr(message);
	if (result != OUTPUT_SUCCESS)
		return (result);
	if (write(STDERR_FILENO, "\n", 1) == -1)
		return (OUTPUT_ERROR_SYSTEM);
	return (OUTPUT_SUCCESS);
}

static t_output_result	impl_write_fd(int fd, const char *message)
{
	if (!message || fd < 0)
		return (OUTPUT_ERROR_WRITE);
	if (write(fd, message, ft_strlen(message)) == -1)
		return (OUTPUT_ERROR_SYSTEM);
	return (OUTPUT_SUCCESS);
}

t_output_service	*create_output_service(void)
{
	t_output_service	*service;

	service = malloc(sizeof(t_output_service));
	if (!service)
		return (NULL);
	service->write_stdout = impl_write_stdout;
	service->write_stderr = impl_write_stderr;
	service->write_stdout_newline = impl_write_stdout_newline;
	service->write_stderr_newline = impl_write_stderr_newline;
	service->write_fd = impl_write_fd;
	return (service);
}

void	destroy_output_service(t_output_service *service)
{
	if (service)
		free(service);
}
