/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:29:59 by caymard           #+#    #+#             */
/*   Updated: 2024/05/16 17:00:37 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

static int	recreate_second_var(t_mini *shell, t_envp *var2, t_envp *temp)
{
	free(var2->name);
	var2->name = ft_strdup(temp->name);
	if (!var2->name)
		return (error_msg(shell, strerror(errno), NULL, NULL), 1);
	free(var2->value);
	if (!temp->value)
		var2->value = NULL;
	else
	{
		var2->value = ft_strdup(temp->value);
		if (!var2->value)
		{
			free_node_env(temp);
			return (error_msg(shell, strerror(errno), NULL, NULL), 1);
		}
	}
	free_node_env(temp);
	return (0);
}

int	swap_var(t_mini *shell, t_envp *var1, t_envp *var2)
{
	t_envp	*temp;

	temp = new_var(shell, var1->name, var1->value, var1->next);
	if (!temp)
		return (1);
	free(var1->name);
	var1->name = ft_strdup(var2->name);
	if (!var1->name)
	{
		free_node_env(temp);
		return (error_msg(shell, strerror(errno), NULL, NULL), 1);
	}
	free(var1->value);
	if (!var2->value)
		var1->value = NULL;
	else
	{
		var1->value = ft_strdup(var2->value);
		if (!var1->value)
		{
			free_node_env(temp);
			return (error_msg(shell, strerror(errno), NULL, NULL), 1);
		}
	}
	return (recreate_second_var(shell, var2, temp));
}
