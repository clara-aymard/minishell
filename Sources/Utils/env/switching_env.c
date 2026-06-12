/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switching_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:55:07 by caymard           #+#    #+#             */
/*   Updated: 2024/05/16 21:34:08 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

char	*update_shlvl(t_mini *shell, char *env)
{
	int		nbr;
	char	*shlvl;

	nbr = ft_atoi(env + 6);
	if (nbr < INT_MIN)
		exit_minishell(shell);
	nbr++;
	shlvl = ft_itoa(nbr);
	if (!shlvl)
		exit_minishell(shell);
	env = strjoin("SHLVL=", shlvl, NULL);
	if (!env)
	{
		free(shlvl);
		exit_minishell(shell);
	}
	free(shlvl);
	return (env);
}

void	get_env_bash(char **env, t_mini *shell)
{
	int	len;
	int	i;

	len = strlen_table(env);
	shell->envv = malloc(sizeof(char *) * (len + 1));
	if (!shell->envv)
	{
		error_msg(0, strerror(errno), NULL, NULL);
		exit_minishell(shell);
	}
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "SHLVL=", 6))
			shell->envv[i] = update_shlvl(shell, env[i]);
		else
			shell->envv[i] = ft_strdup(env[i]);
		if (!shell->envv[i])
		{
			error_msg(0, strerror(errno), NULL, NULL);
			exit_minishell(shell);
		}
		i++;
	}
	shell->envv[i] = 0;
}

void	table_to_list(char **envv, t_mini *shell)
{
	int		i;
	char	**temp;
	t_envp	*new;

	i = 0;
	free_list_env(&(shell->envp));
	while (envv[i])
	{
		temp = split(envv[i], '=');
		if (!temp)
			exit_minishell(shell);
		new = new_var(shell, temp[0], temp[1], NULL);
		if (!new)
		{
			free_table(temp);
			exit_minishell(shell);
		}
		free_table(temp);
		if (!shell->envp)
			shell->envp = new;
		else
			add_last_var(&(shell->envp), new);
		i++;
	}
}

void	recreate_envv(t_envp *envp, t_mini *shell)
{
	t_envp	*temp;
	int		i;

	i = 0;
	temp = envp;
	while (temp)
	{
		if (temp->value)
		{
			shell->envv[i] = strjoin(temp->name, temp->value, "=");
			if (!shell->envv[i])
				exit_minishell(shell);
		}
		else
		{
			shell->envv[i] = ft_strdup(temp->name);
			if (!shell->envv[i])
				exit_minishell(shell);
		}
		temp = temp->next;
		i++;
	}
	shell->envv[i] = NULL;
}

void	list_to_table(t_envp *envp, t_mini *shell)
{
	int		len;

	len = count_len_list(envp);
	if (!len)
		return ;
	free_table(shell->envv);
	shell->envv = malloc(sizeof(char *) * (len + 1));
	if (!shell->envv)
	{
		error_msg(0, strerror(errno), NULL, NULL);
		exit_minishell(shell);
	}
	recreate_envv(envp, shell);
}
