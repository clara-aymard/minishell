/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:57:11 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/22 12:55:01 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

static void	check_input(t_mini *shell, t_lexer **lex_list)
{
	t_lexer	*tmp;

	tmp = get_last_node_lex(lex_list);
	if ((*lex_list)->token == PIPE || tmp->token == PIPE)
	{
		putendl_fd(2, "mini: syntax error near unexpected token `|'");
		shell->status = 258;
		delete_lexlist(lex_list);
		minishell_loop(shell);
	}
	tmp = *lex_list;
	while (tmp)
	{
		if ((tmp->token == INPUT || tmp ->token == TRUNC)
			&& (tmp->next && tmp->next->token != WORD))
		{
			error_msg(shell, "syntax error near unexpected token `",
				tmp->next->str, "'");
			shell->status = 258;
			delete_lexlist(lex_list);
			minishell_loop(shell);
		}
		tmp = tmp->next;
	}
}

static char	*prompt_exe(t_mini *shell)
{
	char	*line;

	av_init_signal();
	if (g_signal == SIGINT)
		shell->status = 130;
	line = readline("\033[1m\033[96mmini\033[0m$ ");
	if (line)
	{
		if (line[0])
		{
			add_history(line);
			return (line);
		}
		free(line);
		return (0);
	}
	putendl_fd(2, "exit");
	exit_minishell(shell);
	return (0);
}

static int	is_valid_input(t_mini *shell, char *input)
{
	if (!input || !input[0])
	{
		if (input)
			free(input);
		return (0);
	}
	if (!is_quote_formatted(input))
	{
		free(input);
		return (0);
	}
	lexer_exe(input, &shell->lex_list, shell);
	if (!shell->lex_list)
	{
		free(input);
		return (0);
	}
	return (1);
}

void	minishell_loop(t_mini *shell)
{
	char	*input;

	shell->lex_list = NULL;
	shell->exec = NULL;
	while (1)
	{
		g_signal = 0;
		input = prompt_exe(shell);
		if (!is_valid_input(shell, input))
			continue ;
		check_input(shell, &shell->lex_list);
		minishell_exe(shell);
		free(input);
	}
}
