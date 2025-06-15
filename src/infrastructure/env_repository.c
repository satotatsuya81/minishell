/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_repository.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/06/16 08:34:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "interfaces/env_repository.h"
#include "usecase/env/env_manager.h"

/* Simplified implementation - environmental variables are managed at exec context level */

t_env_repository	*create_env_repository(void)
{
	t_env_repository	*repo;

	repo = malloc(sizeof(t_env_repository));
	if (!repo)
		return (NULL);
	/* This is a simplified version - functionality moved to exec context */
	repo->add_variable = NULL;
	repo->add_assignment = NULL;
	repo->remove_variable = NULL;
	repo->get_variable = NULL;
	repo->get_all_variables = NULL;
	repo->initialize_from_envp = NULL;
	repo->cleanup = NULL;
	return (repo);
}

void	destroy_env_repository(t_env_repository *repository)
{
	if (repository)
		free(repository);
}
