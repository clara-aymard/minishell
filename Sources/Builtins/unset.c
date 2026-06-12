/* ************************************************************************** */
/*							                                                */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:21:07 by caymard           #+#    #+#             */
/*   Updated: 2024/04/04 20:21:10 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	delete_node(t_envp **head, t_envp *delete, t_mini *shell)
{
	t_envp	*temp;
	t_envp	*next;

	temp = *head;
	if (!ft_strcmp("_", delete->name))
		return ;
	if (delete == *head)
	{
		*head = temp->next;
		free_node_env(temp);
		return ;
	}
	while (temp && temp->next != delete)
		temp = temp->next;
	next = temp->next->next;
	free_node_env(temp->next);
	temp->next = next;
	shell->status = 0;
}

void	unset(t_mini *shell, t_envp *envp, char **vars)
{
	t_envp	*temp;
	int		i;

	i = 0;
	while (vars && vars[++i])
	{
		temp = envp;
		while (temp)
		{
			if (!ft_strcmp(temp->name, vars[i]))
			{
				delete_node(&shell->envp, temp, shell);
				break ;
			}
			temp = temp->next;
			if (!temp && (check_name(vars[i])
					|| ft_strchr(vars[i], '+') || ft_strchr(vars[i], '=')))
				error_msg(shell, "unset: `", vars[i],
					"': not a valid identifier");
			else
				shell->status = 0;
		}
	}
	list_to_table(shell->envp, shell);
}
