/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 20:03:31 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 13:37:45 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void	add_value(t_mini *shell, t_envp *envp, char *name, char *value)
{
	t_envp	*temp;
	char	*old;

	temp = envp;
	while (temp)
	{
		if (!ft_strcmp(temp->name, name)
			&& (ft_strlen(name) == ft_strlen(temp->name)))
		{
			old = temp->value;
			temp->value = strjoin(old, value, NULL);
			free(old);
			if (!temp->value)
				shell->status = 1;
			break ;
		}
		else if (!temp->next)
		{
			temp->next = new_var(shell, name, value, NULL);
			if (!temp->value)
				shell->status = 1;
			break ;
		}
		temp = temp->next;
	}
}

void	replace_value(t_mini *shell, t_envp *envp, char *name, char *value)
{
	t_envp	*temp;
	char	*old;

	temp = envp;
	while (temp)
	{
		if (!ft_strcmp(temp->name, name))
		{
			old = temp->value;
			temp->value = ft_strdup(value);
			if (!temp->value)
				shell->status = 1;
			free(old);
			break ;
		}
		temp = temp->next;
	}
}
