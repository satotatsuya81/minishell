/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_interface.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 08:04:55 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUT_INTERFACE_H
# define OUTPUT_INTERFACE_H

/* Output operation results */
typedef enum e_output_result {
	OUTPUT_SUCCESS,
	OUTPUT_ERROR_WRITE,
	OUTPUT_ERROR_SYSTEM
} t_output_result;

/* Output service interface */
typedef struct s_output_service {
	t_output_result	(*write_stdout)(const char *message);
	t_output_result	(*write_stderr)(const char *message);
	t_output_result	(*write_stdout_newline)(const char *message);
	t_output_result	(*write_stderr_newline)(const char *message);
	t_output_result	(*write_fd)(int fd, const char *message);
} t_output_service;

/* Service factory functions */
t_output_service	*create_output_service(void);
void				destroy_output_service(t_output_service *service);

#endif
