/* ************************************************************************** */
/*				                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:24:48 by caymard           #+#    #+#             */
/*   Updated: 2024/04/05 12:22:35 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	env(t_mini *shell, t_envp *envp, char **av, int fd)
{
	t_envp	*temp;

	temp = envp;
	if (av[1])
	{
		putmsg_fd("env: ", av[1], ": No such file or directory", 2);
		shell->status = 127;
		return ;
	}
	while (temp)
	{
		if (temp->name && temp->value)
			putmsg_fd(temp->name, "=", temp->value, fd);
		temp = temp->next;
	}
	shell->status = 0;
}
