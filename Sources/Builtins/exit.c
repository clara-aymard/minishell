/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:20:49 by caymard           #+#    #+#             */
/*   Updated: 2024/05/16 14:33:05 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	check_space_sign(char *str, int *sign, int *i)
{
	(*i) = 0;
	(*sign) = 1;
	while (((9 <= str[*i]) && (str[*i] <= 13)) || (str[*i] == 32))
		(*i)++;
	if (str[*i] == '-')
		(*sign) = -1;
	if (str[*i] == '-' || str[*i] == '+')
		(*i)++;
}

static long int	ft_atol(char *str, long int *n)
{
	int			i;
	int			sign;
	long int	nbr;
	long int	check;

	nbr = 0;
	check_space_sign(str, &sign, &i);
	while (('0' <= str[i]) && (str[i] <= '9'))
	{
		check = nbr;
		nbr = (nbr * 10) + (str[i] - '0');
		if (nbr / 10 != check)
		{
			if (sign == -1 && str[i] == '8')
			{
				*n = LONG_MIN;
				return (0);
			}
			return (1);
		}
		i++;
	}
	*n = nbr * sign;
	return (0);
}

static int	check_av(char *av)
{
	int	i;

	i = 1;
	if (is_alphanum(av[0]) == 1 || av[0] == '+' || av[0] == '-')
	{
		while (av[i])
		{
			if (is_alphanum(av[i]) != 1)
				return (i);
			i++;
		}
		return (0);
	}
	return (i);
}

static void	status_error_message(t_mini *shell, int status, int i, char *av)
{
	if (i == 0)
		putendl_fd(2, "exit");
	else if (i == 1)
		putendl_fd(2, "minishell: exit: too many arguments");
	else if (i == 2)
	{
		putendl_fd(2, "exit");
		error_msg(shell, "exit: ", av, ": numeric argument required");
	}
	shell->status = status;
}

void	bash_exit(t_mini *shell, char **av)
{
	long int	stat;
	int			check;

	if (!av[1])
		putendl_fd(2, "exit");
	else if (av[1])
	{
		check = ft_atol(av[1], &stat);
		if (!check && !check_av(av[1]))
		{
			if (!check && av[2])
			{
				status_error_message(shell, 1, 1, NULL);
				return ;
			}
			else
				status_error_message(shell, (stat % 256), 0, NULL);
		}
		else
			status_error_message(shell, (127 + 128), 2, av[1]);
	}
	exit_minishell(shell);
}
