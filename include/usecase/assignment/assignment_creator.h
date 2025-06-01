/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_creator.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:00:56 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 16:55:29 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSIGNMENT_CREATOR_H
# define ASSIGNMENT_CREATOR_H

# include "domain/assignment.h"
# include "utils/libft_custom.h"

t_assignment	create_assignment(const char *text, int len);
void			free_assignment(t_assignment *assignment);
#endif
