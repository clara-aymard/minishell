/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:12:43 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 14:22:56 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void	hd_write_line(t_mini *shell, int quoted, int fd, char *line)
{
	char	*tmp;
	char	*expanded;

	if (quoted)
		putendl_fd(fd, line);
	else
	{
		tmp = ft_strdup(line);
		if (!tmp)
			return ;
		expanded = hd_var_expander(shell, tmp);
		if (expanded)
		{
			putendl_fd(fd, expanded);
			free(expanded);
		}
	}
}

static int	hd_delim_len(char *eof)
{
	int	i;
	int	len;
	int	quote;

	i = 0;
	len = 0;
	quote = 0;
	while (eof && eof[i])
	{
		if (!quote && (eof[i] == '\'' || eof[i] == '"'))
			quote = eof[i++];
		else if (quote == eof[i])
		{
			quote = 0;
			i++;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*hd_clean_delimiter(char *eof)
{
	char	*clean;
	int		i;
	int		j;
	int		quote;

	clean = malloc(sizeof(char) * (hd_delim_len(eof) + 1));
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (eof && eof[i])
	{
		if (!quote && (eof[i] == '\'' || eof[i] == '"'))
			quote = eof[i++];
		else if (quote == eof[i])
		{
			quote = 0;
			i++;
		}
		else
			clean[j++] = eof[i++];
	}
	clean[j] = '\0';
	return (clean);
}

void	hd_fork_error(t_mini *shell, int *pipe_fd, char *eof)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	error_msg(shell, strerror(errno), NULL, NULL);
	free(eof);
}

int	open_hd_pipes(t_mini *shell, int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		error_msg(shell, strerror(errno), NULL, NULL);
		return (0);
	}
	return (1);
}
