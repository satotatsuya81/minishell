/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:57:29 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 17:58:58 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usecase/env/env_manager.h"
#include "usecase/assignment/assignment_creator.h"
#include "utils/libft_custom.h"
#include "interfaces/output_interface.h"
#include <stdlib.h>

/**
 * @brief sorts the linked list of environment variables by key.
 *        Algorithm: Bubble sort.
 * @param env Pointer to the head of the linked list of environment variables.
 * @return A new linked list with the same key-value pairs,
 *         or NULL if memory allocation fails.
 */
static void	sort_env_list(t_env_var *env)
{
	t_env_var	*current;
	t_env_var	*next;
	char		*temp_key;
	char		*temp_value;

	current = env;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (ft_strcmp(current->key, next->key) > 0)
			{
				temp_key = current->key;
				current->key = next->key;
				next->key = temp_key;
				temp_value = current->value;
				current->value = next->value;
				next->value = temp_value;
			}
			next = next->next;
		}
		current = current->next;
	}
}

static int	print_env(t_env_var *env, t_output_service *out)
{
	if (!out)
		return (EXIT_FAILURE);
	if (out->write_stdout("declare -x ") != OUTPUT_SUCCESS)
		return (EXIT_FAILURE);
	if (out->write_stdout(env->key) != OUTPUT_SUCCESS)
		return (EXIT_FAILURE);
	if (out->write_stdout("=") != OUTPUT_SUCCESS)
		return (EXIT_FAILURE);
	if (env->value)
	{
		if (out->write_stdout(env->value) != OUTPUT_SUCCESS)
			return (EXIT_FAILURE);
	}
	if (out->write_stdout("\n") != OUTPUT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * @brief Prints the sorted environment variables in the format:
 * declare -x KEY="VALUE"
 * @param env Pointer to the head of the linked list of environment variables.
 * @return EXIT_SUCCESS on success,
 *         EXIT_FAILURE on failure (e.g., if writing to stdout fails).
 */
static int	print_all_env(t_env_var *env, t_output_service *out)
{
	t_env_var	*head;
	t_env_var	*current;

	if (!env)
		return (EXIT_SUCCESS);
	head = dup_env(env);
	if (!head)
		return (EXIT_FAILURE);
	sort_env_list(head);
	current = head;
	while (current)
	{
		if (print_env(current, out) == EXIT_FAILURE)
		{
			env_free(current);
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	env_free(head);
	return (EXIT_SUCCESS);
}

static	void	print_error(char *arg, t_output_service *out)
{
	if (!out)
		return ;
	out->write_stderr("bash: export: `");
	out->write_stderr(arg);
	out->write_stderr("': not a valid identifier\n");
}

/**
 * export with no options
 * @brief Adds or updates environment variables in the linked list.
 * If the variable already exists, it updates its value.
 * If the variable does not exist, it adds a new node with the key and value.
 * @param envp Pointer to the head of the linked list of environment variables.
 */
int	ft_export(char **argv, t_env_var **envp, t_output_service *out)
{
	char			*arg;
	t_assignment	assignment;

	if (!argv || !*argv)
		return (print_all_env(*envp, out));
	while (*argv)
	{
		arg = *argv++;
		assignment = create_assignment(arg, ft_strlen(arg));
		if (assignment.text == NULL)
		{
			print_error(arg, out);
			continue ;
		}
		env_add_assignment(envp, &assignment);
		free_assignment(&assignment);
	}
	return (EXIT_SUCCESS);
}
