/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:33:29 by caymard           #+#    #+#             */
/*   Updated: 2024/05/08 12:28:59 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	putstr_fd(int fd, char *str)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
}

void	putendl_fd(int fd, char *str)
{
	if (!str)
		return ;
	putstr_fd(fd, str);
	write(fd, "\n", 1);
}

void	putmsg_fd(char *str1, char *str2, char *str3, int fd)
{
	putstr_fd(fd, str1);
	putstr_fd(fd, str2);
	putstr_fd(fd, str3);
	write(fd, "\n", 1);
}

void	error_msg(t_mini *shell, char *str1, char *str2, char *str3)
{
	putstr_fd(2, "minishell: ");
	putstr_fd(2, str1);
	putstr_fd(2, str2);
	putstr_fd(2, str3);
	write(2, "\n", 1);
	if (shell)
		shell->status = 1;
}

void	hd_eof_warning(char *eof)
{
	putstr_fd(2, "minishell: warning: here-document delimited by end-of-file");
	putstr_fd(2, " (wanted `");
	putstr_fd(2, eof);
	putendl_fd(2, "')");
}
