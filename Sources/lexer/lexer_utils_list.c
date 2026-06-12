/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:46:36 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/21 17:05:47 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	delete_lexlist(t_lexer **head_ref)
{
	t_lexer	*current;
	t_lexer	*next;

	current = *head_ref;
	if (*head_ref == NULL)
		return ;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	*head_ref = NULL;
}

t_lexer	*get_last_node_lex(t_lexer **lex_list)
{
	t_lexer	*tmp;

	if (!(*lex_list))
		return (NULL);
	tmp = *(lex_list);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	addnode_lexer(t_mini *shell, t_lexer **l_li, t_token token, char *str)
{
	t_lexer	*lex_node;
	t_lexer	*tmp;

	lex_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!lex_node)
		ft_error_parser(2, shell, NULL);
	lex_node->str = str;
	lex_node->token = token;
	lex_node->next = NULL;
	if (*l_li == NULL)
	{
		lex_node->prev = NULL;
		*l_li = lex_node;
	}
	else
	{
		tmp = get_last_node_lex(l_li);
		tmp->next = lex_node;
		lex_node->prev = tmp;
	}
}
