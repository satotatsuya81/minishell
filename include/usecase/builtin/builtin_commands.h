/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:47:42 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 18:02:37 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_COMMANDS_H
# define BUILTIN_COMMANDS_H

# include "domain/env_variable.h"
# include "domain/token.h"

int		ft_cd(char **argv, t_env_var **envp);
int		ft_echo(char **argv);
int		ft_env(t_env_var *envp);
void	ft_exit(char **argv, t_token_stream *stream, t_env_var *env);
int		ft_export(char **argv, t_env_var **envp);
int		ft_pwd(void);
int		ft_unset(char **argv, t_env_var **envp);

#endif
