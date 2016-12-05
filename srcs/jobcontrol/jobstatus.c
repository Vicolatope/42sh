/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobstatus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/27 00:13:31 by jmontija          #+#    #+#             */
/*   Updated: 2016/12/05 05:24:46 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fortytwo.h"

char	*update_status(int sig)
{
	char	*status;

	status = NULL;
	sig == CLD_CONTINUED ? (status = SDUP("CONTINUED")) : 0;
	sig == CLD_STOPPED ? (status = SDUP("SUSPENDED")) : 0;
	sig == CLD_TRAPPED ? (status = SDUP("TRAPPED")) : 0;
	sig == CLD_DUMPED ? (status = SDUP("DUMPED")) : 0;
	sig == CLD_KILLED ? (status = SDUP("INTERRUPT")) : 0;
	sig == CLD_EXITED ? (status = SDUP("EXITED")) : 0;
	status == NULL ? (status = SDUP("UNKNOWN STATUS: TERMINATED")) : 0;
	return (status);
}

void	change_state(t_jobs *jobs, int code)
{
	if (jobs == NULL)
		return ;
	REMOVE(&jobs->status);
	jobs->terminate = code;
	jobs->status = update_status(code);
	jobs->is_last = true;
	if (code > 0)
		display_jobs(jobs, 1);
}

void	jobs_status(t_group *grp)
{
	t_jobs		*jobs;
	//t_jobs		*pipe;
	int			ret;
	int			code;

	jobs = grp->jobs;
	while (jobs)
	{
		code = 0;
		if (jobs->pid > 0 &&
			(jobs->terminate == CLD_STOPPED))
		{	
			ret = waitpid(jobs->pid, &code, WNOHANG | WCONTINUED);
			// if (ret > -1 && WIFCONTINUED(code))
			// 	change_state(jobs, CLD_CONTINUED);
			// pipe = jobs->next_pipe;
			// while (pipe)
			// {
			// 	ret = waitpid(pipe->pid, &code, WNOHANG | WUNTRACED | WCONTINUED);
			// 	printf("%d\n", code);
			// 	if (ret > -1 && WIFCONTINUED(code))
			// 		change_state(pipe, CLD_CONTINUED);
			// 	else if (ret > -1 && WIFSTOPPED(code))
			// 		change_state(pipe, CLD_STOPPED);
			// 	else if (ret > -1 && WIFEXITED(code))
			// 		change_state(pipe, CLD_EXITED);
			// 	else if (ret == 2)
			// 		change_state(pipe, CLD_KILLED);
			// 	pipe = pipe->next_pipe;
			// }
		}
		jobs = jobs->next;
	}
}

void	remove_job(t_jobs *jobs)
{
	REMOVE(&jobs->cmd);
	REMOVE(&jobs->status);
	jobs->pid = -1;
	jobs->terminate = -1;
	jobs->is_last = false;
	jobs->is_prelast = false;
}

void	jobs_update(t_group *grp)
{
	t_jobs		*jobs;

	jobs = grp->jobs;
	while (jobs)
	{
		if (jobs->pid > 0)
		{
			if (jobs->terminate > -1 && jobs->terminate != CLD_STOPPED
				&& jobs->terminate != CLD_CONTINUED)
				remove_job(jobs);
		}
		jobs = jobs->next;
	}
	// update les + - nil
}