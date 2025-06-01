/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_initializer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:54:44 by tatsato           #+#    #+#             */
/*   Updated: 2025/05/31 21:51:53 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INITIALIZER_H
# define ENV_INITIALIZER_H

# include "domain/env_variable.h"

t_env_var	*env_create_from_envp(char **envp);
void		env_free(t_env_var *env);

#endif
