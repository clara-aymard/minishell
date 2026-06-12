/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 22:34:41 by caymard           #+#    #+#             */
/*   Updated: 2024/05/21 13:53:09 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	close_std_backup(t_mini *shell)
{
	if (shell->m_stdin >= 0)
		close(shell->m_stdin);
	if (shell->m_stdout >= 0)
		close(shell->m_stdout);
}

void	clean_child(t_mini *shell)
{
	free_table(shell->envv);
	shell->envv = NULL;
	free_list_env(&(shell->envp));
	shell->envp = NULL;
	free_list_exec(&(shell->exec));
	shell->exec = NULL;
	rl_clear_history();
}

void	close_parent_fds(t_exec *exec, int *fd)
{
	close(fd[0]);
	if (exec->parse.fd_in >= 0)
		close(exec->parse.fd_in);
	if (exec->parse.fd_out >= 0)
		close(exec->parse.fd_out);
}

void	wait_pipeline(t_mini *shell, pid_t pid)
{
	t_exec	*temp;
	int		status;

	temp = shell->exec;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) > 0)
		shell->status = WEXITSTATUS(status);
	while (temp->next)
	{
		wait(NULL);
		temp = temp->next;
	}
}
