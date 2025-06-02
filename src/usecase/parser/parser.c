/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirokugo <mirokugo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/01 17:48:15 by mirokugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/usecase/parser/parser_internal.h"
#include "../../../src/utils/libft/libft.h"

/* 新しいパイプラインを作成してパース */
t_pipeline *create_and_parse_pipeline(t_token_stream *tokens, t_parse_result *result)
{
    t_pipeline *new_pipeline = create_pipeline();
    if (!new_pipeline)
        return NULL;
    
    t_cmd *cmds = parse_pipeline(tokens, result);
    if (!cmds) {
        free_pipeline(new_pipeline);
        return NULL;
    }
    
    new_pipeline->cmds = cmds;
    return new_pipeline;
}

/* 演算子を処理 */
int handle_operator(t_token_stream *tokens, t_pipeline *pipeline, t_parse_result *result)
{
    if (!tokens->current || tokens->current->type != TOKEN_OPERATOR)
        return 1;
    
    pipeline->connector = get_connector_type(tokens->current->value.word);
    tokens->current = tokens->current->next;
    
    if (!tokens->current || tokens->current->type == TOKEN_EOF) {
        result->error_msg = ft_strdup("syntax error: unexpected end after operator");
        return 0;
    }
    return 1;
}

/* パイプラインをリストに追加 */
int add_pipeline_to_list(t_pipeline **first, t_pipeline **current, t_pipeline *new_pipeline)
{
    if (!*first) {
        *first = new_pipeline;
        *current = new_pipeline;
    } else {
        (*current)->next = new_pipeline;
        *current = new_pipeline;
    }
    return 1;
}

/* メインループでパイプラインを処理 */
t_parse_result *parse_main_loop(t_token_stream *tokens, t_parse_result *result)
{
    t_pipeline *first_pipeline = NULL;
    t_pipeline *current_pipeline = NULL;
    
    while (tokens->current && tokens->current->type != TOKEN_EOF) {
        t_pipeline *new_pipeline = create_and_parse_pipeline(tokens, result);
        if (!new_pipeline) {
            if (!handle_parse_error(result, first_pipeline))
                return NULL;
            return result;
        }
        
        add_pipeline_to_list(&first_pipeline, &current_pipeline, new_pipeline);
        
        if (!handle_operator(tokens, current_pipeline, result)) {
            free_pipeline(first_pipeline);
            result->ast = NULL;
            return result;
        }
    }
    
    result->ast = first_pipeline;
    return result;
}

/* メインのパース関数 - 複数のパイプラインを演算子で接続 */
t_parse_result *parse(t_token_stream *tokens)
{
    if (!tokens || !tokens->head)
        return NULL;
    
    t_parse_result *result = create_parse_result();
    if (!result)
        return NULL;
    
    tokens->current = tokens->head;
    return parse_main_loop(tokens, result);
}