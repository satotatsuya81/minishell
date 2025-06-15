/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_dto.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/16 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_DTO_H
# define COMMAND_DTO_H

/* Command redirection DTO for cross-layer communication */
typedef struct s_redirection_dto {
	int						type;
	char					*file;
	int						fd;
	struct s_redirection_dto *next;
} t_redirection_dto;

/* Command DTO for cross-layer communication */
typedef struct s_command_dto {
	char					**argv;
	t_redirection_dto		*redirections;
	struct s_command_dto	*next;
} t_command_dto;

/* Pipeline DTO for cross-layer communication */
typedef struct s_pipeline_dto {
	t_command_dto			*commands;
	struct s_pipeline_dto	*next;
	int						connector;
} t_pipeline_dto;

/* Parse result DTO for cross-layer communication */
typedef struct s_parse_result_dto {
	t_pipeline_dto	*pipelines;
	char			*error_message;
	int				error_line;
	int				error_column;
} t_parse_result_dto;

/* DTO creation and cleanup functions */
t_command_dto		*create_command_dto(char **argv);
t_redirection_dto	*create_redirection_dto(int type, char *file, int fd);
t_pipeline_dto		*create_pipeline_dto(t_command_dto *commands, int connector);
t_parse_result_dto	*create_parse_result_dto(t_pipeline_dto *pipelines, 
						char *error_msg);

void				free_command_dto(t_command_dto *cmd);
void				free_redirection_dto(t_redirection_dto *redir);
void				free_pipeline_dto(t_pipeline_dto *pipeline);
void				free_parse_result_dto(t_parse_result_dto *result);

#endif