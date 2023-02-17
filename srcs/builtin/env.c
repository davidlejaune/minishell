/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 18:07:50 by dly               #+#    #+#             */
/*   Updated: 2023/02/17 18:24:33 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env(t_data *data, t_proc *proc)
{
	while(data->env)
	{
		ft_putendl_fd(data->env->content, proc->fd_out);
		data->env = data->env->next;
	}
	return (0);
}