/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:24:38 by caymard           #+#    #+#             */
/*   Updated: 2024/05/22 13:04:44 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void	exec_cd(t_mini *shell, char *path)
{
	if (chdir(path) == -1)
	{
		putstr_fd(2, "minishell: cd: ");
		putstr_fd(2, path);
		putstr_fd(2, ": ");
		putendl_fd(2, strerror(errno));
		shell->status = 1;
		return ;
	}
	shell->status = 0;
}

static void	get_pwd_value(t_mini *shell, t_envp **path, t_envp *temp)
{
	char	cwd[PATH_MAX];

	if (!temp)
		*path = new_var(shell, "PWD", NULL, NULL);
	shell->status = 0;
	(*path)->value = ft_strdup(getcwd(cwd, sizeof(cwd)));
	if (!(*path)->value || !ft_strcmp((*path)->value, "-1"))
		exit_minishell(shell);
}

void	get_var(t_mini *shell, t_envp *envp, char *var, t_envp **path)
{
	t_envp	*temp;

	temp = envp;
	while (temp && ft_strcmp(temp->name, var))
	{
		temp = temp->next;
	}
	if ((ft_strcmp(var, "PWD")) && (!temp || (temp && !temp->value)))
	{
		error_msg(shell, "cd: ", var, " not set");
		return ;
	}
	shell->status = 0;
	*path = temp;
	if (!ft_strcmp(var, "PWD") && (!temp || (temp && !temp->value)))
		get_pwd_value(shell, path, temp);
}

int	check_oldpwd(t_mini *shell, t_envp *envp, t_envp **old)
{
	t_envp	*temp;

	temp = envp;
	while (temp && ft_strcmp(temp->name, "OLDPWD"))
		temp = temp->next;
	if (temp)
	{
		*old = temp;
		if (temp->value)
			return (2);
		return (1);
	}
	*old = new_var(shell, "OLDPWD", NULL, NULL);
	if (!*old)
		exit_minishell(shell);
	add_last_var(&(shell->envp), *old);
	return (0);
}

void	change_old_pwd(t_mini *shell, t_envp *envp, char *path)
{
	t_envp	*pwd;
	t_envp	*oldpwd;
	char	cwd[PATH_MAX];
	int		old_state;

	exec_cd(shell, path);
	if (shell->status == 1)
		return ;
	get_var(shell, envp, "PWD", &pwd);
	if (shell->status == 1)
		return ;
	old_state = check_oldpwd(shell, envp, &oldpwd);
	if (old_state == 2)
		free(oldpwd->value);
	oldpwd->value = ft_strdup(pwd->value);
	if (!oldpwd->value)
		exit_minishell(shell);
	free(pwd->value);
	pwd->value = ft_strdup(getcwd(cwd, sizeof(cwd)));
	if (!pwd->value || !ft_strcmp(pwd->value, "-1"))
		exit_minishell(shell);
}
