/* ************************************************************************** */
/*											                                */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:24:35 by caymard           #+#    #+#             */
/*   Updated: 2024/04/05 11:10:34 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	pwd(t_mini *shell, int fd)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		putmsg_fd("minishell: pwd: ", strerror(errno), NULL, 2);
		exit_minishell(shell);
	}
	else
	{
		putendl_fd(fd, cwd);
		shell->status = 0;
	}
}
