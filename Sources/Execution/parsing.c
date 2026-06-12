/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:19:31 by caymard           #+#    #+#             */
/*   Updated: 2024/05/21 13:11:07 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	get_cmd_path(t_mini *shell, char *cmd, t_parse *parse)
{
	int			i;

	i = 0;
	while (parse->paths[i])
	{
		parse->path_cmd = strjoin(parse->paths[i], cmd, "/");
		if (!parse->path_cmd)
		{
			error_msg(shell, strerror(errno), NULL, NULL);
			return ;
		}
		if (access(parse->path_cmd, X_OK) == 0)
		{
			shell->status = 0;
			return ;
		}
		free(parse->path_cmd);
		parse->path_cmd = NULL;
		i++;
	}
	shell->status = 127;
	putmsg_fd("minishell: ", cmd, ": command not found", 2);
	parse->path_cmd = NULL;
}

void	av_is_absolute(t_mini *shell, t_exec *exec, t_parse *parse)
{
	struct stat	path_stat;

	parse->path_cmd = ft_strdup(exec->av[0]);
	if (!parse->path_cmd)
	{
		error_msg(shell, strerror(errno), NULL, NULL);
		shell->status = 1;
		return ;
	}
	if (stat(exec->av[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		error_msg(shell, exec->av[0], ": is a directory", NULL);
		shell->status = 126;
		return ;
	}
	if (access(exec->av[0], X_OK) == 0)
		shell->status = 0;
	else
	{
		error_msg(shell, exec->av[0], ": No such file or directory", NULL);
		shell->status = 127;
	}
}

void	exec_or_absolute(t_mini *shell, t_exec *exec, t_parse *parse)
{
	struct stat	path_stat;

	if (!ft_strncmp("...", exec->av[0], 3))
	{
		error_msg(shell, exec->av[0], ": No such file or directory", NULL);
		shell->status = 127;
	}
	else if (!ft_strncmp("..", exec->av[0], 2))
	{
		if (stat(exec->av[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		{
			error_msg(shell, exec->av[0], ": is a directory", NULL);
			shell->status = 126;
			return ;
		}
		error_msg(shell, exec->av[0], ": No such file or directory", NULL);
		shell->status = 127;
	}
	else if (!ft_strncmp("/", exec->av[0], 1)
		|| !ft_strncmp(".", exec->av[0], 1))
		av_is_absolute(shell, exec, parse);
}

void	get_paths(t_mini *shell, char **envv, t_exec *exec, t_parse *parse)
{
	int		i;

	if (!ft_strncmp(".", exec->av[0], 1) || !ft_strncmp("/", exec->av[0], 1))
	{
		exec_or_absolute(shell, exec, parse);
		return ;
	}
	i = 0;
	while (envv[i] && ft_strncmp("PATH=", envv[i], 5))
		i++;
	if (!envv[i])
	{
		shell->status = 127;
		putmsg_fd("minishell: ", exec->av[0], ": No such file or directory", 2);
		return ;
	}
	parse->paths = split(envv[i] + 5, ':');
	if (!parse->paths)
	{
		putmsg_fd("minishell: ", strerror(errno), NULL, 2);
		shell->status = 1;
		return ;
	}
	get_cmd_path(shell, exec->av[0], parse);
}
