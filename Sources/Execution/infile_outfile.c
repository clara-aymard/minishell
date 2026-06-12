/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:20:05 by caymard           #+#    #+#             */
/*   Updated: 2024/05/15 17:43:58 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	check_infile(t_mini *shell, t_file *temp, t_exec *exec, int *count)
{
	int		fd;

	if (temp->type == INPUT)
	{
		if (*count == -2)
		{
			fd = open(temp->file, O_RDONLY);
			if (fd == -1)
				error_msg(shell, temp->file, ": ", strerror(errno));
			return ;
		}
		if (*count)
			close(exec->parse.fd_in);
		exec->parse.fd_in = open(temp->file, O_RDONLY);
		if (exec->parse.fd_in == -1)
		{
			error_msg(shell, temp->file, ": ", strerror(errno));
			shell->status = 1;
			exec->parse.fd_in = -2;
			return ;
		}
		(*count)++;
	}
	else if (temp->type == HERE_DOC && (*count != -2 && *count))
		close(exec->parse.fd_in);
}

static void	check_outfile(t_mini *shell, t_file *temp, t_exec *exec, int *count)
{
	if (*count)
		close(exec->parse.fd_out);
	if (temp->type == APPEND)
		exec->parse.fd_out = open(temp->file, O_WRONLY
				| O_CREAT | O_APPEND, 0644);
	else
		exec->parse.fd_out = open(temp->file, O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
	if (exec->parse.fd_out == -1)
	{
		putstr_fd(2, "minishell: ");
		putmsg_fd(temp->file, ": ", strerror(errno), 2);
		shell->status = 1;
		return ;
	}
	(*count)++;
}

static void	check_here_doc(t_exec exec, int *count)
{
	t_file	*t_file;

	t_file = exec.file;
	if (!t_file)
		return ;
	while (t_file)
	{
		if (t_file->type == HERE_DOC)
		{
			*count = -2;
			t_file = t_file->next;
			while (t_file)
			{
				if (t_file->type == HERE_DOC || t_file->type == INPUT)
				{
					*count = 0;
					return ;
				}
				t_file = t_file->next;
			}
			if (!t_file)
				return ;
		}
		t_file = t_file->next;
	}
}

static void	get_file(t_mini *shell, t_exec *exec)
{
	t_file	*file;
	int		count_in;
	int		count_out;

	get_here_doc(shell, exec);
	if (shell->status == 1)
		return ;
	while (exec)
	{
		file = exec->file;
		count_in = 0;
		count_out = 0;
		check_here_doc(*exec, &count_in);
		while (file)
		{
			if (file->type == INPUT || file->type == HERE_DOC)
				check_infile(shell, file, exec, &count_in);
			else if (file->type == TRUNC || file->type == APPEND)
				check_outfile(shell, file, exec, &count_out);
			if (shell->status == 1)
				break ;
			file = file->next;
		}
		exec = exec->next;
	}
}

void	check_file(t_mini *shell, t_exec *exec)
{
	t_exec	*temp;

	shell->status = 0;
	temp = exec;
	while (temp)
	{
		if (temp->file)
		{
			get_file(shell, temp);
			return ;
		}
		temp = temp->next;
	}
	shell->status = 0;
}
