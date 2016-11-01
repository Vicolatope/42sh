/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/30 16:51:54 by jmontija          #+#    #+#             */
/*   Updated: 2016/10/30 18:43:55 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fortytwo.h"

void	show_env(int choice, t_group *grp)
{
	t_envlst	*tmp;

	tmp = choice == 0 ? ENV(lst) : ENV(lst_tmp);
	while (tmp)
	{
		ft_putstr_fd(tmp->name, 2);
		ft_putstr_fd("=", 2);
		ft_putendl_fd(tmp->val, 2);
		tmp = tmp->next;
	}
}

void	create_tmp_env(t_group *grp)
{
	t_envlst	*tmp;
	char		*var;
	char		*var_tmp;

	tmp = ENV(lst);
	while (tmp)
	{
		var = JOIN(tmp->name, "=");
		var_tmp = JOINF(var, tmp->val, 1);
		insert_env_tmp(grp, var_tmp);
		REMOVE(&var_tmp);
		tmp = tmp->next;
	}
}

void	change_cmd(t_group *grp)
{
	int	i;

	i = -1;
	REMOVE(&CMD(cmd));
	CMD(cmd) = SDUP(ENV(cmd));
	while (CMD(cmdsplit)[++i])
		REMOVE(&(CMD(cmdsplit)[i]));
	free(CMD(cmdsplit));
	CMD(cmdsplit) = ft_strsplit(CMD(cmd), ' ');
	REMOVE(&ENV(cmd));
	printf("curr command to exec: %s\n", CMD(cmd));
}

int		builtin_env(t_group *grp)
{
	char	**cmdsplit;
	int		start;
	int		end;

	cmdsplit = CMD(cmdsplit);
	if (env_opt(grp) < 0)
		return (-1);
	if (ENV(opt_i) == false && ENV(start_varenv) == false && ENV(cmd) == NULL)
		show_env(0, grp);
	else
	{
		ENV(opt_i) == false ? create_tmp_env(grp) : 0;
		start = ENV(start_varenv);
		end = start + ENV(end_varenv);
		while (start < end)
		{
			insert_env_tmp(grp, cmdsplit[start]);
			start++;
		}
		ENV(cmd) == NULL ? show_env(1, grp) : change_cmd(grp);
	}
	return (1);
}
