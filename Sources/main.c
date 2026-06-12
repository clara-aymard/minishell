/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:53:06 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/22 12:53:45 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	cpy_std_fileno(t_mini *shell)
{
	shell->m_stdin = dup(STDIN_FILENO);
	if (shell->m_stdin == -1)
	{
		error_msg(shell, strerror(errno), NULL, NULL);
		exit_minishell(shell);
	}
	shell->m_stdout = dup(STDOUT_FILENO);
	if (shell->m_stdout == -1)
	{
		error_msg(shell, strerror(errno), NULL, NULL);
		exit_minishell(shell);
	}
}

int	main(int ac, char **av, char **envbash)
{
	t_mini	shell;

	(void)av;
	if (ac != 1)
	{
		putendl_fd(1, "Too many arguments");
		return (1);
	}
	shell.envv = NULL;
	if (!strlen_table(envbash))
		add_basic_vars(&shell);
	else
	{
		get_env_bash(envbash, &shell);
		shell.envp = NULL;
		table_to_list(shell.envv, &shell);
		check_basic_vars(&shell, shell.envp);
	}
	shell.status = 0;
	cpy_std_fileno(&shell);
	minishell_loop(&shell);
	return (0);
}
