/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_fct.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:55:23 by caymard           #+#    #+#             */
/*   Updated: 2024/05/16 14:30:44 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int	count_len_list(t_envp *envp)
{
	t_envp	*temp;
	int		i;

	if (!envp)
		return (0);
	temp = envp;
	i = 0;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

t_envp	*last_node(t_envp *envp)
{
	t_envp	*temp;

	temp = envp;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	add_last_var(t_envp **a, t_envp *new)
{
	if (!a)
		return ;
	if (!*a)
		*a = new;
	else
		last_node(*a)->next = new;
}
