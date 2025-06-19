/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:30:00 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/18 15:30:00 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

# include <signal.h>

/* Signal handler functions */
void	setup_signal_handlers(void);
void	setup_child_signal_handlers(void);
void	ignore_signals(void);
void	restore_signals(void);

/* Signal context for readline - Required global for signal safety */
extern volatile sig_atomic_t	g_signal_received;

#endif