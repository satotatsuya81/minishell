/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_interface.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:04:35 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_INTERFACE_H
# define IO_INTERFACE_H

# include <stdbool.h>

/* I/O operation results */
typedef enum e_io_result {
	IO_SUCCESS,
	IO_ERROR_ACCESS,
	IO_ERROR_NOT_FOUND,
	IO_ERROR_PERMISSION,
	IO_ERROR_SYSTEM
} t_io_result;

/* File system operations interface */
typedef struct s_io_service {
	t_io_result	(*change_directory)(const char *path);
	char		*(*get_current_directory)(void);
	bool		(*file_exists)(const char *path);
	bool		(*is_directory)(const char *path);
	t_io_result	(*create_directory)(const char *path);
	char		*(*get_error_message)(t_io_result result);
} t_io_service;

/* Service factory functions */
t_io_service	*create_io_service(void);
void			destroy_io_service(t_io_service *service);

#endif
