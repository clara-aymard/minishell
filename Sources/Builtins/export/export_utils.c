/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:20:09 by caymard           #+#    #+#             */
/*   Updated: 2024/05/16 20:04:07 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

t_envp	*new_var(t_mini *shell, char *name, char *value, t_envp *next)
{
	t_envp	*new_var;

	new_var = malloc(sizeof(t_envp));
	if (!new_var)
		return (error_msg(shell, strerror(errno), NULL, NULL), NULL);
	new_var->name = ft_strdup(name);
	if (!new_var->name)
	{
		free_node_env(new_var);
		return (error_msg(shell, strerror(errno), NULL, NULL), NULL);
	}
	if (!value)
	{
		new_var->value = NULL;
		new_var->next = next;
		return (new_var);
	}
	new_var->value = ft_strdup(value);
	if (!new_var->value)
	{
		free_node_env(new_var);
		return (error_msg(shell, strerror(errno), NULL, NULL), NULL);
	}
	new_var->next = next;
	return (new_var);
}

int	is_alphanum(char c)
{
	if (('0' <= c) && (c <= '9'))
		return (1);
	else if (('A' <= c) && (c <= 'Z'))
		return (2);
	else if (('a' <= c) && (c <= 'z'))
		return (3);
	else if (c == '_')
	{
		return (4);
	}
	return (0);
}

int	check_name(char *name)
{
	int	i;

	if (!is_alphanum(name[0]) || is_alphanum(name[0]) == 1)
		return (1);
	i = 1;
	if (!name[i])
		return (0);
	while (name[i] && (name[i] != '+') && (name[i] != '='))
	{
		if (!is_alphanum(name[i]))
			return (1);
		i++;
	}
	if (name[i] == '+')
	{
		if (!name[i + 1] && (name[i + 1] != '='))
			return (1);
	}
	return (0);
}

int	check_exist(char *name, t_envp *envp)
{
	t_envp	*temp;
	char	*value;
	int		len_name;

	temp = envp;
	value = ft_strchr(name, '+');
	if (!value)
	{
		value = ft_strchr(name, '=');
	}
	len_name = ft_strlen(name) - ft_strlen(value);
	while (temp)
	{
		if (!ft_strncmp(temp->name, name, ft_strlen(temp->name))
			&& (ft_strlen(temp->name) == len_name))
			return (1);
		temp = temp->next;
	}
	return (0);
}
