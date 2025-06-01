/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirokugo <mirokugo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:27:11 by mirokugo          #+#    #+#             */
/*   Updated: 2025/06/01 17:50:27 by mirokugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/interfaces/parser_internal.h"
#include "../../../src/utils/libft/libft.h"

/* argv配列を拡張 */
char **extend_argv(char **argv, int current_size)
{
    char **new_argv = malloc(sizeof(char *) * (current_size + 2));
    if (!new_argv)
        return NULL;
    
    for (int i = 0; i < current_size; i++) {
        new_argv[i] = argv[i];
    }
    new_argv[current_size] = NULL;
    new_argv[current_size + 1] = NULL;
    
    free(argv);
    return new_argv;
}

/* 演算子のタイプを判定 */
t_connector get_connector_type(const char *op)
{
    if (!op)
        return CONN_NONE;
    
    if (ft_strncmp(op, "&&", 2) == 0)
        return CONN_AND;
    else if (ft_strncmp(op, "||", 2) == 0)
        return CONN_OR;
    else if (ft_strncmp(op, ";", 1) == 0)
        return CONN_SEMICOLON;
    
    return CONN_NONE;
}

/* パースエラーを処理 */
int handle_parse_error(t_parse_result *result, t_pipeline *first_pipeline)
{
    free_pipeline(first_pipeline);
    if (!result->error_msg) {
        free_parse_result(result);
        return 0;
    }
    result->ast = NULL;
    return 1;
}

/* パース結果のメモリを解放 */
void free_parse_result(t_parse_result *result)
{
    if (!result)
        return;
    
    free_pipeline(result->ast);
    free(result->error_msg);
    free(result);
}