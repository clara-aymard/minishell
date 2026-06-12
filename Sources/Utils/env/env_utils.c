/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 20:19:33 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 17:23:26 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void	add_basic_vars(t_mini *shell)
{
	char	cwd[PATH_MAX];

	shell->envp = new_var(shell, "OLDPWD", NULL, NULL);
	if (shell->status == 1)
		exit_minishell(shell);
	getcwd(cwd, sizeof(cwd));
	add_var(shell, shell->envp, "PWD", cwd);
	if (shell->status == 1)
		exit_minishell(shell);
	add_var(shell, shell->envp, "SHLVL", "1");
	if (shell->status == 1)
		exit_minishell(shell);
	add_var(shell, shell->envp, "_", "/usr/bin/env");
	if (shell->status == 1)
		exit_minishell(shell);
	list_to_table(shell->envp, shell);
}

void	apply_basic_vars(t_mini *shell, int i, int j, int k)
{
	t_envp	*new;
	char	cwd[PATH_MAX];

	if (!i)
	{
		new = new_var(shell, "OLDPWD", NULL, NULL);
		if (!new)
			exit_minishell(shell);
		add_last_var(&(shell->envp), new);
	}
	if (!j)
	{
		new = new_var(shell, "PWD", getcwd(cwd, sizeof(cwd)), NULL);
		if (!new)
			exit_minishell(shell);
		add_last_var(&(shell->envp), new);
	}
	if (!k)
	{
		new = new_var(shell, "SHLVL", "1", NULL);
		if (!new)
			exit_minishell(shell);
		add_last_var(&(shell->envp), new);
	}
}

void	check_basic_vars(t_mini *shell, t_envp *envp)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (envp)
	{
		if (!ft_strncmp("OLDPWD", envp->name, ft_strlen(envp->name)))
			i = 1;
		if (!ft_strncmp("PWD", envp->name, ft_strlen(envp->name)))
			j = 1;
		if (!ft_strncmp("SHLVL", envp->name, ft_strlen(envp->name)))
			k = 1;
		envp = envp->next;
	}
	if (!i || !j || !k)
		apply_basic_vars(shell, i, j, k);
	list_to_table(shell->envp, shell);
}
