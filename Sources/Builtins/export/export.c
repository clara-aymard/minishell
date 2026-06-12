/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:20:25 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 13:38:15 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void	add_var(t_mini *shell, t_envp *envp, char *name, char *value)
{
	t_envp	*temp;

	temp = envp;
	while (temp->next)
		temp = temp->next;
	temp->next = new_var(shell, name, value, NULL);
	if (!temp->next)
		return ;
	shell->status = 0;
}

static void	var_exist(t_mini *shell, t_envp *envp, char *var, char *sep)
{
	char	**temp;

	if (sep && (sep[1] == '='))
	{
		temp = split(var, '+');
		if (!temp)
			return (error_msg(shell, strerror(errno), NULL, NULL));
		if (!temp[1] || temp[1][1] == '\0')
			add_value(shell, envp, temp[0], "");
		else
			add_value(shell, envp, temp[0], (temp[1] + 1));
		free_table(temp);
	}
	else if (!sep && ft_strchr(var, '='))
	{
		temp = split(var, '=');
		if (!temp)
			return (error_msg(shell, strerror(errno), NULL, NULL));
		if (!temp[1])
			add_value(shell, envp, temp[0], "");
		else
			replace_value(shell, envp, temp[0], temp[1]);
		free_table(temp);
	}
}

static void	var_not_exist(t_mini *shell, t_envp *envp, char *var, char *sep)
{
	char	**temp;

	if (sep && (sep[1] == '='))
	{
		temp = split(var, '+');
		if (!temp)
			return (error_msg(shell, strerror(errno), NULL, NULL));
		if (!temp[1] || temp[1][1] == '\0')
			add_var(shell, envp, temp[0], "");
		else
			add_var(shell, envp, temp[0], (temp[1] + 1));
		return (free_table(temp));
	}
	else if (!sep && ft_strchr(var, '='))
	{
		temp = split(var, '=');
		if (!temp)
			return (error_msg(shell, strerror(errno), NULL, NULL));
		if (!temp[1])
			add_var(shell, envp, temp[0], "");
		else
			add_var(shell, envp, temp[0], temp[1]);
		return (free_table(temp));
	}
	add_var(shell, envp, var, NULL);
}

void	export_check(t_mini *shell, t_envp *envp, char **vars, int i)
{
	char	*sep;

	sep = ft_strchr(vars[i], '+');
	if (!check_name(vars[i]) && check_exist(vars[i], envp) == 1)
	{
		var_exist(shell, envp, vars[i], sep);
		if (shell->status == 1)
			exit_minishell(shell);
	}
	else if (!check_name(vars[i]) && check_exist(vars[i], envp) != 1)
	{
		var_not_exist(shell, envp, vars[i], sep);
		if (shell->status == 1)
			exit_minishell(shell);
	}
	else
		error_msg(shell, "export: `", vars[i], "': not a valid identifier");
}

void	export(t_mini *shell, t_envp *envp, char **vars, int fd)
{
	int		i;

	if (!vars[1])
		print_export(shell, envp, fd);
	else
	{
		i = 1;
		while (vars[i])
		{
			shell->status = 0;
			if (ft_strcmp("_", vars[i]))
				export_check(shell, envp, vars, i);
			i++;
		}
	}
	list_to_table(shell->envp, shell);
}
