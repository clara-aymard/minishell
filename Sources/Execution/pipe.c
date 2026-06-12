/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:41:35 by caymard           #+#    #+#             */
/*   Updated: 2024/05/21 13:40:48 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	child_process(t_mini *shell, t_exec *exec, t_parse *parse, int *fd)
{
	handle_signal(2);
	close(fd[0]);
	if (parse->fd_in >= 0)
	{
		if (dup2(parse->fd_in, STDIN_FILENO) == -1)
			return (error_msg(shell, "dup2: ", strerror(errno), NULL));
		close(parse->fd_in);
	}
	if (parse->fd_out >= 0)
	{
		if (dup2(parse->fd_out, STDOUT_FILENO) == -1)
			return (error_msg(shell, "dup2: ", strerror(errno), NULL));
		close(parse->fd_out);
	}
	else if (exec->next && exec->parse.fd_out == -1)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			return (error_msg(shell, "dup2: ", strerror(errno), NULL));
	}
	close(fd[1]);
}

static void	run_child_process(t_mini *shell, t_exec *exec,
	t_parse *parse, int *fd)
{
	close_std_backup(shell);
	if (exec->parse.fd_in == -2 || exec->parse.fd_out == -2)
	{
		close(fd[0]);
		close(fd[1]);
		clean_child(shell);
		exit(1);
	}
	child_process(shell, exec, parse, fd);
	if (!exec->has_builtin && exec->av)
	{
		execve(parse->path_cmd, exec->av, shell->envv);
		clean_child(shell);
		exit(shell->status);
	}
	else if (exec->has_builtin && exec->has_builtin != 2
		&& exec->has_builtin != 5 && exec->has_builtin != 7)
		exec_builtin(shell, exec, STDOUT_FILENO);
	clean_child(shell);
	exit(0);
}

static void	parent_process(t_mini *shell, t_exec *exec, int *fd, pid_t pid)
{
	handle_signal(2);
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		error_msg(shell, "dup2: ", strerror(errno), NULL);
		return ;
	}
	close_parent_fds(exec, fd);
	if (!exec->next)
		wait_pipeline(shell, pid);
}

static void	multiple_process(t_mini *shell, t_exec *exec, t_parse *parse)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (error_msg(shell, "pipe: ", strerror(errno), NULL));
	pid = fork();
	if (pid == -1)
		return (error_msg(shell, "fork: ", strerror(errno), NULL));
	if (pid == 0)
		run_child_process(shell, exec, parse, fd);
	else
		parent_process(shell, exec, fd, pid);
}

void	pipe_check(t_mini *shell, t_exec *exec)
{
	if (!exec->next && exec->has_builtin)
		get_builtin_alone(shell, exec);
	else
	{
		while (exec)
		{
			if (!exec->has_builtin && exec->av)
			{
				get_paths(shell, shell->envv, exec, &(exec->parse));
				if (exec->parse.path_cmd)
					multiple_process(shell, exec, &(exec->parse));
			}
			else
				multiple_process(shell, exec, &(exec->parse));
			exec = exec->next;
		}
		if (g_signal == SIGINT)
			shell->status = 130;
		else if (g_signal == SIGQUIT)
			shell->status = 131;
	}
}
