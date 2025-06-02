/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirokugo <mirokugo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/01 21:45:00 by mirokugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/usecase/parser/parser_internal.h"
#include "../../../src/utils/libft/libft.h"

/* パースエラーを設定 */
void set_parse_error(t_parse_result *result, t_token_stream *tokens, const char *msg)
{
    result->error_msg = ft_strdup(msg);
    if (tokens->current) {
        result->error_line = tokens->current->line_number;
        result->error_column = tokens->current->column;
    }
}

/* 最初のコマンドエラーを処理 */
void handle_first_cmd_error(t_parse_result *result, t_token_stream *tokens)
{
    if (tokens->current && tokens->current->type == TOKEN_PIPE) {
        set_parse_error(result, tokens, "syntax error near unexpected token '|'");
    } else if (tokens->current && tokens->current->type == TOKEN_REDIRECT) {
        set_parse_error(result, tokens, "syntax error near unexpected token 'newline'");
    }
}

/* パイプコマンドを処理 */
int process_pipe_commands(t_token_stream *tokens, t_parse_result *result, t_cmd **current_cmd)
{
    while (tokens->current && tokens->current->type == TOKEN_PIPE) {
        tokens->current = tokens->current->next;
        
        t_cmd *new_cmd = parse_simple_command(tokens);
        if (!new_cmd) {
            set_parse_error(result, tokens, "syntax error: unexpected end after pipe");
            return 0;
        }
        
        (*current_cmd)->next = new_cmd;
        *current_cmd = new_cmd;
    }
    return 1;
}

/* パイプラインのパース（複数のコマンドをパイプで接続） */
t_cmd *parse_pipeline(t_token_stream *tokens, t_parse_result *result)
{
    t_cmd *first_cmd = parse_simple_command(tokens);
    if (!first_cmd) {
        handle_first_cmd_error(result, tokens);
        return NULL;
    }
    
    t_cmd *current_cmd = first_cmd;
    
    if (!process_pipe_commands(tokens, result, &current_cmd)) {
        free_cmd(first_cmd);
        return NULL;
    }
    
    return first_cmd;
}