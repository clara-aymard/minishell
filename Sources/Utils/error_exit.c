/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:01:54 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/17 15:10:36 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	ft_error_parser(int flag, t_mini *shell, char **tab_str)
{
	if (flag == 1)
	{
		free_list_exec(&(shell->exec));
		shell->exec = NULL;
		delete_lexlist(&shell->lex_list);
		shell->lex_list = NULL;
		shell->status = 258;
		putendl_fd(2, "mini: syntax error near unexpected token `newline'");
		minishell_loop(shell);
	}
	if (flag == 2)
	{
		delete_lexlist(&shell->lex_list);
		shell->lex_list = NULL;
		free_list_exec(&(shell->exec));
		shell->exec = NULL;
		free_malloc_split(tab_str);
		tab_str = NULL;
		putendl_fd(2, "minishell: malloc failed");
		exit_minishell(shell);
	}
}

void	free_list_exec(t_exec **exec)
{
	t_exec	*temp;

	if (!exec || (exec && !(*exec)))
		return ;
	while (*exec)
	{
		temp = *exec;
		if (!temp->has_builtin && temp->av)
		{
			free_table(temp->parse.paths);
			free(temp->parse.path_cmd);
		}
		free_table(temp->av);
		free_list_file(temp->file);
		*exec = (*exec)->next;
		free(temp);
	}
}

void	exit_minishell(t_mini *shell)
{
	free_table(shell->envv);
	free_list_env(&(shell->envp));
	free_list_exec(&(shell->exec));
	rl_clear_history();
	close(shell->m_stdin);
	close(shell->m_stdout);
	exit(shell->status % 256);
}
