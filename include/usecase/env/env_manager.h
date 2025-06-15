/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/16 00:00:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_MANAGER_H
# define ENV_MANAGER_H

# include "domain/env_variable.h"
# include "domain/assignment.h"

/* Environment management functions */
void		env_add(t_env_var **env,
				const char *key, const char *value, const char *operator);
void		env_add_assignment(t_env_var **env, t_assignment *assignment);
void		env_remove(t_env_var **env, const char *key);
char		*env_get(const t_env_var *env, const char *key);
t_env_var	*dup_env(const t_env_var *env);

/* Environment initialization */
t_env_var	*env_create_from_envp(char **envp);
void		env_free(t_env_var *env);

#endif