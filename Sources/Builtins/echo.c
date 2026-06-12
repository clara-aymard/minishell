/* ************************************************************************** */
/*									                                        */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 11:27:20 by caymard           #+#    #+#             */
/*   Updated: 2024/04/05 11:28:58 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	is_n_option(char *str)
{
	int	i;

	i = 0;
	if (!str || str[i] != '-')
		return (0);
	i++;
	if (str[i] != 'n')
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] != '\0')
		return (0);
	return (1);
}

void	echo(t_mini *shell, char **av, int fd)
{
	int	newline;
	int	i;
	int	ac;

	ac = strlen_table(av);
	i = 1;
	newline = 1;
	while (i < ac && is_n_option(av[i]))
	{
		newline = 0;
		i++;
	}
	while (i < ac)
	{
		putstr_fd(fd, av[i]);
		if (i < ac - 1)
			putstr_fd(fd, " ");
		i++;
	}
	if (newline)
		putstr_fd(fd, "\n");
	shell->status = 0;
}
