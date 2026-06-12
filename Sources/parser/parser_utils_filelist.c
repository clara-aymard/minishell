/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_filelist.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:57:20 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/17 14:37:14 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	delete_filelist(t_file **file_list)
{
	t_file	*tmp;

	if (!file_list)
		return ;
	tmp = (*file_list);
	while ((*file_list))
	{
		tmp = (*file_list);
		free(tmp->file);
		(*file_list) = (*file_list)->next;
		free(tmp);
	}
	*file_list = NULL;
}

static t_file	*get_lastnode_filelist(t_file **file_list)
{
	t_file	*tmp;

	if (!file_list)
		return (NULL);
	tmp = *(file_list);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

int	addnode_flist(t_file **f_list, t_token token, char *file, int quote)
{
	t_file	*file_node;
	t_file	*tmp;

	file_node = (t_file *)malloc(sizeof(t_file));
	if (!file_node)
		return (-1);
	file_node->type = token;
	file_node->file = ft_strdup(file);
	if (!file_node->file)
		return (-1);
	file_node->quotted_heredoc = quote;
	file_node->next = NULL;
	if (*f_list == NULL)
		*f_list = file_node;
	else
	{
		tmp = get_lastnode_filelist(f_list);
		tmp->next = file_node;
	}
	return (0);
}
