/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 11:08:32 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 13:39:17 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

static void	child_hd_exit(t_mini *shell, char *eof, int status)
{
	free(eof);
	free_table(shell->envv);
	free_list_env(&(shell->envp));
	free_list_exec(&(shell->exec));
	rl_clear_history();
	exit(status);
}

static void	child_here_doc(t_mini *shell, char *eof, int quoted, int *fd)
{
	char	*line;

	close(fd[0]);
	handle_signal(4);
	line = readline("> ");
	while (line && ft_strcmp(line, eof))
	{
		hd_write_line(shell, quoted, fd[1], line);
		free(line);
		line = readline("> ");
	}
	if (!line)
		hd_eof_warning(eof);
	free(line);
	close(fd[1]);
	child_hd_exit(shell, eof, 0);
}

static void	parent_here_doc(t_mini *shell, t_exec *exec, int *fd, pid_t pid)
{
	int		wstatus;

	close(fd[1]);
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 130)
	{
		close(fd[0]);
		g_signal = SIGINT;
		shell->status = 130;
		return ;
	}
	if (exec->parse.fd_in >= 0)
		close(exec->parse.fd_in);
	exec->parse.fd_in = dup(fd[0]);
	if (exec->parse.fd_in == -1)
		error_msg(shell, strerror(errno), NULL, NULL);
	close(fd[0]);
}

static void	run_here_doc_child(t_mini *shell, t_file *file,
	char *eof, int *pipe_fd)
{
	if (shell->m_stdin >= 0)
		close(shell->m_stdin);
	if (shell->m_stdout >= 0)
		close(shell->m_stdout);
	child_here_doc(shell, eof, file->quotted_heredoc, pipe_fd);
}

void	here_doc_exec(t_mini *shell, t_file *file, t_exec *exec)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	*eof;

	eof = hd_clean_delimiter(file->file);
	if (!eof)
		return ;
	if (!open_hd_pipes(shell, pipe_fd))
	{
		free(eof);
		return ;
	}
	handle_signal(3);
	pid = fork();
	if (pid == -1)
		return (hd_fork_error(shell, pipe_fd, eof));
	if (pid == 0)
		run_here_doc_child(shell, file, eof, pipe_fd);
	else
		parent_here_doc(shell, exec, pipe_fd, pid);
	free(eof);
}
