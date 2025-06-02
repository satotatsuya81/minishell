/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirokugo <mirokugo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/01 17:50:30 by mirokugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/usecase/parser/parser_internal.h"
#include "../../../src/utils/libft/libft.h"

/* 単語をコマンドに追加 */
int add_word_to_cmd(t_cmd *cmd, t_token_stream *tokens, int *argc)
{
    cmd->argv = extend_argv(cmd->argv, *argc);
    if (!cmd->argv)
        return 0;
    
    cmd->argv[*argc] = ft_strdup(tokens->current->value.word);
    if (!cmd->argv[*argc])
        return 0;
    
    (*argc)++;
    tokens->current = tokens->current->next;
    return 1;
}

/* リダイレクションをコマンドに追加 */
int add_redirect_to_cmd(t_cmd *cmd, t_token_stream *tokens, t_cmd_redirect **last_redirect)
{
    t_cmd_redirect *new_redirect = parse_redirection(tokens);
    if (!new_redirect)
        return 0;
    
    if (!cmd->redirects) {
        cmd->redirects = new_redirect;
        *last_redirect = new_redirect;
    } else {
        (*last_redirect)->next = new_redirect;
        *last_redirect = new_redirect;
    }
    return 1;
}

/* 単純なコマンドを初期化 */
t_cmd *init_simple_command(void)
{
    t_cmd *cmd = create_cmd();
    if (!cmd)
        return NULL;
    
    cmd->argv = malloc(sizeof(char *));
    if (!cmd->argv) {
        free(cmd);
        return NULL;
    }
    cmd->argv[0] = NULL;
    return cmd;
}

/* コマンドトークンを処理 */
int process_command_tokens(t_cmd *cmd, t_token_stream *tokens, int *argc, t_cmd_redirect **last_redirect)
{
    while (tokens->current && 
           (tokens->current->type == TOKEN_WORD || 
            tokens->current->type == TOKEN_REDIRECT)) {
        
        if (tokens->current->type == TOKEN_WORD) {
            if (!add_word_to_cmd(cmd, tokens, argc))
                return 0;
        } else if (tokens->current->type == TOKEN_REDIRECT) {
            if (!add_redirect_to_cmd(cmd, tokens, last_redirect))
                return 0;
        }
    }
    return 1;
}

/* 単純なコマンドのパース（リダイレクション対応） */
t_cmd *parse_simple_command(t_token_stream *tokens)
{
    t_cmd *cmd = init_simple_command();
    if (!cmd)
        return NULL;
    
    int argc = 0;
    t_cmd_redirect *last_redirect = NULL;
    
    if (!process_command_tokens(cmd, tokens, &argc, &last_redirect)) {
        free_cmd(cmd);
        return NULL;
    }
    
    if (argc == 0) {
        free_cmd(cmd);
        return NULL;
    }
    
    return cmd;
}