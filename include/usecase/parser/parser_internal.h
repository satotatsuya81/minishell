/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirokugo <mirokugo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/01 21:45:00 by mirokugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include "parser_interface.h"
#include "../../domain/command.h"
#include "../../domain/token.h"

/* Factory functions */
t_parse_result *create_parse_result(void);
t_pipeline *create_pipeline(void);
t_cmd *create_cmd(void);
t_cmd_redirect *create_redirect(t_redirect_type type, const char *file);

/* Utility functions */
char **extend_argv(char **argv, int current_size);
t_connector get_connector_type(const char *op);
void free_parse_result(t_parse_result *result);

/* Parser functions */
t_cmd_redirect *parse_redirection(t_token_stream *tokens);
t_cmd *parse_simple_command(t_token_stream *tokens);
t_cmd *parse_pipeline(t_token_stream *tokens, t_parse_result *result);

/* Parser helper functions */
t_pipeline *create_and_parse_pipeline(t_token_stream *tokens, t_parse_result *result);
int handle_operator(t_token_stream *tokens, t_pipeline *pipeline, t_parse_result *result);
int add_pipeline_to_list(t_pipeline **first, t_pipeline **current, t_pipeline *new_pipeline);
int handle_parse_error(t_parse_result *result, t_pipeline *first_pipeline);
t_parse_result *parse_main_loop(t_token_stream *tokens, t_parse_result *result);

/* Command parser helper functions */
int add_word_to_cmd(t_cmd *cmd, t_token_stream *tokens, int *argc);
int add_redirect_to_cmd(t_cmd *cmd, t_token_stream *tokens, t_cmd_redirect **last_redirect);
t_cmd *init_simple_command(void);
int process_command_tokens(t_cmd *cmd, t_token_stream *tokens, int *argc, t_cmd_redirect **last_redirect);

/* Pipeline parser helper functions */
void set_parse_error(t_parse_result *result, t_token_stream *tokens, const char *msg);
void handle_first_cmd_error(t_parse_result *result, t_token_stream *tokens);
int process_pipe_commands(t_token_stream *tokens, t_parse_result *result, t_cmd **current_cmd);

#endif /* PARSER_INTERNAL_H */