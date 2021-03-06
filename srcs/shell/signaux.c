/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/27 03:04:04 by jmontija          #+#    #+#             */
/*   Updated: 2016/12/14 18:17:03 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fortytwo.h"

void	handler_win(int sig)
{
	t_group			*grp;
	struct winsize	w;

	grp = get_grp();
	if (sig)
		;
	ioctl(0, TIOCGWINSZ, &w);
	TERM(window->width) = w.ws_col;
	TERM(window->heigth) = w.ws_row;
}

void	handler_ctrl_c(int sig)
{
	t_group	*grp;

	grp = get_grp();
	if (sig && grp->comp)
		comp_free(grp, &grp->comp);
	ft_go_end(grp);
	ft_free_parse(grp);
	check_parentheses(0);
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
	TERM(other_read) = false;
	REMOVE(&TERM(cmd_line));
	grp->prompt_size = 6;
	ioctl(0, TIOCSTI, "\n");
}

void	handler_sigcont(int sig)
{
	t_group	*grp;

	if (sig)
		;
	grp = get_grp();
	ft_putendl_fd("42sh: exited", 2);
	ft_exit(grp, 0);
}

void	sig_handler(void)
{
	signal(SIGINT, handler_ctrl_c);
	signal(SIGQUIT, handler_ctrl_c);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGWINCH, handler_win);
	signal(SIGCONT, handler_sigcont);
}
