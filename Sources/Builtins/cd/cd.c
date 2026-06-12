/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:27:25 by caymard           #+#    #+#             */
/*   Updated: 2024/05/22 13:12:14 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void	exchange_pwd_oldpwd(t_mini *shell, t_envp *envp)
{
	t_envp	*oldpwd;
	t_envp	*pwd;
	char	*path;

	get_var(shell, envp, "OLDPWD", &oldpwd);
	if (shell->status == 1)
		return ;
	if (!oldpwd->value)
	{
		error_msg(shell, "cd: ", "OLDPWD not set", NULL);
		shell->status = 1;
		return ;
	}
	path = ft_strdup(oldpwd->value);
	if (!path)
		exit_minishell(shell);
	change_old_pwd(shell, envp, path);
	free(path);
	if (shell->status == 1)
		return ;
	get_var(shell, envp, "PWD", &pwd);
	if (shell->status == 1)
		return ;
	putendl_fd(1, pwd->value);
}

static void	run_cd(t_mini *shell, t_envp *envp, char **av, int argc)
{
	t_envp	*home;

	if (argc == 1)
	{
		get_var(shell, envp, "HOME", &home);
		if (shell->status == 1)
			return ;
		change_old_pwd(shell, envp, home->value);
	}
	else if (!ft_strcmp("-", av[1]))
		exchange_pwd_oldpwd(shell, envp);
	else
		change_old_pwd(shell, envp, av[1]);
}

void	cd(t_mini *shell, t_envp *envp, char **av)
{
	int	argc;

	argc = strlen_table(av);
	if (argc > 2)
	{
		error_msg(shell, "cd: ", "too many arguments", NULL);
		shell->status = 1;
		return ;
	}
	run_cd(shell, envp, av, argc);
	if (shell->status == 1)
		return ;
	list_to_table(envp, shell);
	shell->status = 0;
}
