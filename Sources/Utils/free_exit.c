/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:23:29 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 15:10:26 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

char	**free_malloc_split(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (NULL);
	while (ptr[i] && ptr[i][0] != '\0')
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (NULL);
}

void	free_table(char **table)
{
	int	i;

	i = 0;
	if (!table || (table && !(*table)))
		return ;
	while (table[i])
	{
		free(table[i]);
		i++;
	}
	free(table);
	return ;
}

void	free_node_env(t_envp *node)
{
	free(node->name);
	free(node->value);
	free(node);
}

void	free_list_env(t_envp **envp)
{
	t_envp	*temp;

	if (!envp || (envp && !*envp))
		return ;
	while (*envp)
	{
		temp = *envp;
		*envp = (*envp)->next;
		free_node_env(temp);
	}
}

void	free_list_file(t_file *file)
{
	t_file	*temp;

	if (!file)
		return ;
	while (file)
	{
		temp = file;
		file = (file)->next;
		free(temp->file);
		temp->file = NULL;
		free(temp);
	}
}
