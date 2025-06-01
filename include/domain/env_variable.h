/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:54:44 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:50:52 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_VARIABLE_H
# define ENV_VARIABLE_H

# include "domain/assignment.h"

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}						t_env_var;

void		env_add(t_env_var **env,
				const char *key, const char *value, const char *operator);
void		env_add_assignment(t_env_var **env, t_assignment *assignment);
void		env_remove(t_env_var **env, const char *key);
char		*env_get(const t_env_var *env, const char *key);
t_env_var	*dup_env(const t_env_var *env);

#endif
