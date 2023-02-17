/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:51:18 by dly               #+#    #+#             */
/*   Updated: 2023/02/17 13:01:00by dly              ###   ########.fr       */
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
	t_proc	*idx_wait;
	t_proc	*tmp;

	idx_wait = proc;
	while (proc)
	{
		// call function again to create a subshell and assign pipe for the last cmd of the subshell to the next cmd
		// with a check if a redir is used
		if (proc->type == SUBSHELL)
		{
			tmp = get_last_proc(proc->procs);
			if (proc->next_pipeline == PIPE)
			{
				if (tmp->fd_out == STDOUT_FILENO)
					tmp->fd_out = tmp->pipes[1];
				if (proc->next->type == SUBSHELL && proc->next->procs->fd_in == STDIN_FILENO)
				
					proc->next->procs->fd_in = tmp->pipes[0];
				else if (proc->next->fd_in == STDIN_FILENO)
					proc->next->fd_in = tmp->pipes[0];
			}
			child(data, proc->procs);
			proc = proc->next;
		}
		// create a child if it's a CMD
		if (proc->type == COMMAND)
		{
			if (proc->next_pipeline == PIPE)
			{
				if (proc->fd_out == STDOUT_FILENO)
				{
					proc->fd_out = proc->pipes[1];
					if (proc->next->fd_in == STDIN_FILENO)
						proc->next->fd_in = proc->pipes[0];
				}
			}
			proc->pid = fork();
			if (!proc->pid)
			{
				// if (proc->next_pipeline == PIPE)
				// {
				// 	if (proc->fd_out == STDOUT_FILENO)
				// 	{
				// 		if (proc->next->fd_in == STDIN_FILENO)
				// 			proc->next->fd_in = proc->pipes[0];
				// 		proc->fd_out = proc->pipes[1];
				// 	}
					if (double_dup2(proc->fd_in, proc->fd_out) == -1)
							exit(1);
				// }
				if (close_pipe(idx_wait) == -1)
					exit(1);
				if (!access(proc->path, 0))
					execve(proc->path, ft_lst_to_tab(proc->args) ,ft_lst_to_tab(data->env));
				exit(0);
			}
		}
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	while(idx_wait)
	{
		if (idx_wait->type == COMMAND)
			waitpid(idx_wait->pid, &idx_wait->exit_status, 0);
		if (idx_wait->next_pipeline == AND || idx_wait->next_pipeline == OR)
			break ;
		idx_wait = idx_wait->next;
	}
	if (proc)
		child(data, proc->next);
	return (0);
}

// int	child(t_data *data, t_proc *proc)
// {
// 	// open_pipe(proc);
// 	int infile = open("Makefile", O_RDONLY);
// 	if (infile == -1)
// 	{
// 		write(2,"error", 5);
// 	}

// 	pipe(proc->pipes);
// 	proc->pid = fork();
// 	char **env = ft_lst_to_tab(data->env);
// 	if (!proc->pid)
// 	{
// 		dup2(proc->pipes[1], 1);
// 		// close(proc->pipes[0]);
// 		close(proc->pipes[1]);
// 		// if (close_pipe(proc) == -1)
// 			// exit(1);
// 		if (!access(proc->path, 0))
// 			execve(proc->path, ft_lst_to_tab(proc->args) , env);
// 		write(2, "error\n",6);
// 		exit(1);
// 	}
// 	// waitpid(-1, NULL, 0);
// 	usleep(100);
// 	proc->next->pid = fork();
// 	if (!proc->next->pid)
// 	{
// 		dup2(proc->pipes[0], STDIN_FILENO);
// 		// dup2(proc->pipes[0], 0);
// 		close(proc->pipes[0]);
// 		close(proc->pipes[1]);
// 		// if (close_pipe(proc) == -1)
// 			// exit(2);
// 		if (!access(proc->path, 0))
// 		{
// 			execve(proc->next->path, ft_lst_to_tab(proc->next->args) ,env);
// 			write(2, "error\n",6);
// 		}
// 		write(2, "error\n",6);
// 	}
// 	usleep(10000);
// 	// waitpid(-1, NULL, 0);// 
// 	waitpid(proc->pid, &proc->exit_status, WNOHANG);
// 	waitpid(proc->next->pid, &proc->next->exit_status, WNOHANG);
// 	printf("%d     %d\n", proc->exit_status, proc->next->exit_status);
// 	return 0;
// }