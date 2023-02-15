/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:51:18 by dly               #+#    #+#             */
/*   Updated: 2023/02/15 19:19:15 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_pipe(t_proc *proc)
{
	while (proc)
	{
		if (proc->type == SUBSHELL)
			open_pipe(proc->procs);
		if (proc->next_pipeline == PIPE) 
		{
			if (pipe(proc->pipes) == -1)
				return (-1);
		}
		proc = proc->next;
	}
	return (0);
}

int	double_dup2(int in, int out)
{
	if (dup2(in, STDIN_FILENO) == -1)
		return (-1);
	if (dup2(out, STDOUT_FILENO) == -1)
		return (-1);
	return (0);
}

int	close_pipe(t_proc *proc)
{
	while (proc)
	{
		if (proc->type == SUBSHELL)
			close_pipe(proc->procs);
		if (proc->next_pipeline == PIPE) 
		{
			if (close(proc->pipes[0]) == -1)
				return (-1);
			if (close(proc->pipes[1]) == -1)
				return (-1);
		}
		proc = proc->next;
	}
	return (0);
}

int	child(t_data *data, t_proc *proc)
{
	int	i;
	t_proc *tmp;

	i = 0;
	open_pipe(proc);	
	while (proc)
	{
		tmp = proc;
		if (proc->type == SUBSHELL)
			child(data, proc->procs);
		{
			proc->pid = fork();
			i++;

		}
		if (!proc->pid)
		{
			if (proc->next_pipeline == PIPE)
			{
				proc->next->fd_in = proc->pipes[0];
				if (double_dup2(proc->fd_in, proc->pipes[1]) == -1)
					exit(1);
			}
				if (dup2(proc->pipe[0], STDIN_FILENO) == -1)
				exit(1);
			if (close_pipe(proc) == -1)
				exit(1);
			if (!access(proc->path, 0))
				execve(proc->path, ft_lst_to_tab(proc->args) ,ft_lst_to_tab(data->env));
			exit(0);
		}
		if (proc->pid == -1)
		{

		}
		
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
		{
			while (1)
			{
				waitpid(tmp->pid, NULL, 0);
				if (tmp->next_pipeline != AND && tmp->next_pipeline != OR)
				{
					tmp = tmp->next;
					break ;
				}
			}
		}
		proc = proc->next;
	}
	
	waitpid(-1, NULL, 0);
	return (0);
}

int	child(t_data *data, t_proc *proc)
{
	t_proc *tmp;

	tmp = proc;
	open_pipe(proc);
	while (proc)
	{
		if (proc->type == SUBSHELL)
			child(data, proc->procs);
		if (proc->next_pipeline != AND && proc->next_pipeline != OR) 
		proc->pid = fork();

		if (!proc->pid)
		{

		}
		if (tmp->next_pipeline == AND || tmp->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	while(tmp)
	{
		waitpid(tmp->pid, NULL, 0);
		if (tmp->next_pipeline == AND || tmp->next_pipeline == OR)
			break ;
		tmp = tmp->next;
	}
	if (proc)
		child(data, proc);
}
