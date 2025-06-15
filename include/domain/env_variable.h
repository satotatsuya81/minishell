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

/* Forward declaration to avoid circular dependency */
typedef struct s_assignment t_assignment;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}						t_env_var;

/* Function declarations moved to usecase layer */
/* These functions will be defined in usecase/env/ */

#endif
