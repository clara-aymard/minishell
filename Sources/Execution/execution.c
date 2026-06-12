/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:08:21 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 13:39:16 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	reset_std_exec(t_mini *shell)
{
	if (dup2(shell->m_stdin, STDIN_FILENO) == -1)
		error_msg(shell, strerror(errno), NULL, NULL);
	if (dup2(shell->m_stdout, STDOUT_FILENO) == -1)
		error_msg(shell, strerror(errno), NULL, NULL);
	free_list_exec(&(shell->exec));
}

void	exec_builtin(t_mini *shell, t_exec *exec, int out_fd)
{
	if (!ft_strcmp(exec->av[0], "export"))
		export(shell, shell->envp, exec->av, out_fd);
	else if (!ft_strcmp(exec->av[0], "cd"))
		cd(shell, shell->envp, exec->av);
	else if (!ft_strcmp(exec->av[0], "echo"))
		echo(shell, exec->av, out_fd);
	else if (!ft_strcmp(exec->av[0], "env"))
		env(shell, shell->envp, exec->av, out_fd);
	else if (!ft_strcmp(exec->av[0], "exit"))
		bash_exit(shell, exec->av);
	else if (!ft_strcmp(exec->av[0], "pwd"))
		pwd(shell, out_fd);
	else if (!ft_strcmp(exec->av[0], "unset"))
		unset(shell, shell->envp, exec->av);
}

void	get_builtin_alone(t_mini *shell, t_exec *exec)
{
	if (exec->parse.fd_in >= 0)
	{
		if (dup2(exec->parse.fd_in, STDIN_FILENO) == -1)
		{
			shell->status = 1;
			putmsg_fd("minishell: dup2: ", strerror(errno), NULL, 2);
			return ;
		}
		close(exec->parse.fd_in);
	}
	if (exec->parse.fd_out >= 0)
	{
		if (dup2(exec->parse.fd_out, STDOUT_FILENO) == -1)
		{
			shell->status = 1;
			putmsg_fd("minishell: dup2: ", strerror(errno), NULL, 2);
			return ;
		}
		close(exec->parse.fd_out);
	}
	exec_builtin(shell, exec, STDOUT_FILENO);
}

void	sort_execution(t_mini *shell, t_exec *exec)
{
	check_file(shell, shell->exec);
	if (shell->status == 1 && !exec->next)
	{
		reset_std_exec(shell);
		return ;
	}
	if (exec->av || exec->next)
		pipe_check(shell, exec);
	reset_std_exec(shell);
}
