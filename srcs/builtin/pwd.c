/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 17:20:38 by dly               #+#    #+#             */
/*   Updated: 2023/02/17 17:43:10 by dly              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	pwd(t_proc *proc)
{
	char	buf[PATH_MAX];

	if (!getcwd(buf, PATH_MAX))
		return (-1);
	ft_putstr_fd(buf, proc->fd_out);
	return (0);
}