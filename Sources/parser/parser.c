/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:41:12 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/21 17:20:12 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	is_builtins(t_exec **exec_list, char *str)
{
	if (!str[0] && !str)
		return ;
	else if ((!ft_strncmp(str, "echo", 4) && ft_strlen(str) == 4))
		(*exec_list)->has_builtin = 1;
	else if ((!ft_strncmp(str, "cd", 2) && ft_strlen(str) == 2))
		(*exec_list)->has_builtin = 2;
	else if ((!ft_strncmp(str, "pwd", 3) && ft_strlen(str) == 3))
		(*exec_list)->has_builtin = 3;
	else if ((!ft_strncmp(str, "export", 6) && ft_strlen(str) == 6))
		(*exec_list)->has_builtin = 4;
	else if ((!ft_strncmp(str, "unset", 5) && ft_strlen(str) == 5))
		(*exec_list)->has_builtin = 5;
	else if ((!ft_strncmp(str, "env", 3) && ft_strlen(str) == 3))
		(*exec_list)->has_builtin = 6;
	else if ((!ft_strncmp(str, "exit", 4) && ft_strlen(str) == 4))
		(*exec_list)->has_builtin = 7;
	else
		return ;
}

static char	**getcmd(t_mini *sh, t_lexer **l_li, t_exec **e_list, int count)
{
	char	**list_cmd;
	t_lexer	*tmp;
	int		i;

	if (*l_li == NULL || (*l_li && !(*l_li)->str[0] && (*l_li)->next == NULL))
		return (NULL);
	tmp = *l_li;
	list_cmd = (char **)malloc(sizeof(char *) * (count + 1));
	i = 0;
	if (!list_cmd)
		ft_error_parser(2, sh, NULL);
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->str[0])
		{
			list_cmd[i] = ft_strdup(tmp->str);
			if (!list_cmd[i++])
				ft_error_parser(2, sh, list_cmd);
		}
		delnode_lexlist(l_li, tmp);
		tmp = *l_li;
	}
	is_builtins(e_list, list_cmd[0]);
	list_cmd[i] = 0;
	return (list_cmd);
}

t_lexer	*hand_red(t_lexer **l_li, t_file **f_li, t_lexer *l_node, t_mini *sh)
{
	t_lexer			*next_node;
	t_lexer			*next_next_node;
	t_expand_tools	exp_tools;
	char			*str;

	if (!l_node->next)
		ft_error_parser(1, sh, NULL);
	init_exp_tools(&exp_tools, NULL);
	if (is_quoted(l_node->next->str))
	{
		str = del_quote_bslash(sh, &exp_tools, l_node->next->str);
		if (addnode_flist(f_li, l_node->token, str, 1) == -1)
			ft_error_parser(2, sh, NULL);
		if (exp_tools.is_quote == 1)
			free(str);
	}
	else
		if (addnode_flist(f_li, l_node->token, l_node->next->str, 0) == -1)
			ft_error_parser(2, sh, NULL);
	next_node = l_node->next;
	next_next_node = next_node->next;
	delnode_lexlist(l_li, next_node);
	delnode_lexlist(l_li, l_node);
	return (next_next_node);
}

t_lexer	*hand_pipe(t_mini *shell, t_lexer *tmp, int *n, t_exec **e_node)
{
	if (!shell->lex_list->str[0] && (shell->lex_list->next
			&& shell->lex_list->next->token == PIPE))
	{
		tmp = tmp->next;
		delnode_lexlist(&shell->lex_list, tmp->prev->prev);
		delnode_lexlist(&shell->lex_list, tmp->prev);
		*n = 0;
		return (tmp);
	}
	addnode_execlist(shell, &shell->exec);
	if ((shell->lex_list) && (shell->lex_list)->token != PIPE)
		(*e_node)->av = getcmd(shell, &shell->lex_list, e_node, *n);
	*n = 0;
	*e_node = (*e_node)->next;
	tmp = tmp->next;
	delnode_lexlist(&shell->lex_list, tmp->prev);
	return (tmp);
}

void	parser_exe(t_lexer **l_li, t_exec **e_li, t_mini *shell, int *n)
{
	t_lexer	*tmp_lex;
	t_exec	*tmp_exec;

	*n = 0;
	addnode_execlist(shell, e_li);
	tmp_exec = *e_li;
	tmp_lex = *l_li;
	while (tmp_lex)
	{
		if (!tmp_lex->str[0])
			tmp_lex = tmp_lex->next;
		else if (tmp_lex->token == PIPE)
			tmp_lex = hand_pipe(shell, tmp_lex, n, &tmp_exec);
		else if (is_redirect_token(tmp_lex->token))
			tmp_lex = hand_red(l_li, &tmp_exec->file, tmp_lex, shell);
		else
		{
			tmp_lex = tmp_lex->next;
			(*n)++;
		}
	}
	tmp_exec->av = getcmd(shell, l_li, &tmp_exec, *n);
}
