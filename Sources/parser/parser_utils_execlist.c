/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_execlist.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:07:23 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/17 14:37:28 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	delete_execlist(t_exec **exec_list)
{
	t_exec	*current;
	t_exec	*next;

	current = *exec_list;
	if (*exec_list == NULL)
		return ;
	while (current != NULL)
	{
		next = current->next;
		free_table(current->av);
		delete_filelist(&current->file);
		free(current->file);
		free(current);
		current = next;
	}
	*exec_list = NULL;
}

void	delnode_lexlist(t_lexer **lex_list, t_lexer *del)
{
	if (*lex_list == NULL || del == NULL)
		return ;
	if (*lex_list == del)
		*lex_list = del->next;
	if (del->next != NULL)
		del->next->prev = del->prev;
	if (del->prev != NULL)
		del->prev->next = del->next;
	free(del->str);
	free(del);
}

static t_exec	*get_last_node_execlist(t_exec **exec_list)
{
	t_exec	*tmp;

	if (!exec_list)
		return (NULL);
	tmp = *(exec_list);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	addnode_execlist(t_mini *shell, t_exec **exec_list)
{
	t_exec	*exec_node;
	t_exec	*tmp;

	exec_node = (t_exec *)malloc(sizeof(t_exec));
	if (!exec_node)
		ft_error_parser(2, shell, NULL);
	exec_node->file = NULL;
	exec_node->av = NULL;
	exec_node->has_builtin = 0;
	exec_node->next = NULL;
	exec_node->parse.fd_in = -1;
	exec_node->parse.fd_out = -1;
	exec_node->parse.paths = NULL;
	exec_node->parse.path_cmd = NULL;
	if (*exec_list == NULL)
	{
		exec_node->prev = NULL;
		*exec_list = exec_node;
	}
	else
	{
		tmp = get_last_node_execlist(exec_list);
		tmp->next = exec_node;
		exec_node->prev = tmp;
	}
}
