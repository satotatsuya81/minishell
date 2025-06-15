/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/13 23:19:59 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUT_UTILS_H
# define OUTPUT_UTILS_H

# include "domain/command.h"
# include "adapters/parser/parser_interface.h"
# include "usecase/executor/executor.h"

void	print_indent(int depth);
void	print_pipeline_cmds(t_pipeline *ast, int depth);
void	print_ast_debug(t_pipeline *ast, int depth);
void	print_ast(t_pipeline *ast);
void	print_execution_summary(int status, t_exec_context *ctx);

#endif