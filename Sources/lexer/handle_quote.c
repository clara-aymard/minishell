/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:59:40 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/17 15:08:20 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int	is_escaped(char *str, int index)
{
	int	backslash_count;

	backslash_count = 0;
	while (index > 0 && str[index - 1] == '\\')
	{
		backslash_count++;
		index--;
	}
	if (backslash_count % 2 == 0)
		return (0);
	else
		return (1);
}

static int	is_good_quote(char *str, char *ch)
{
	char	*tmp;
	int		i;

	tmp = str;
	i = 0;
	while (*tmp)
	{
		if ((*tmp == '"' || *tmp == '\'') && !is_escaped(str, i))
		{
			*ch = *tmp;
			tmp = ft_strchr_premium(str, (tmp + 1), *tmp, &i);
			if (!tmp)
				return (0);
		}
		i++;
		tmp++;
	}
	return (1);
}

int	is_quote_formatted(char *str)
{
	char	ch;

	if (!is_good_quote(str, &ch))
	{
		putstr_fd(2, "syntax_error missing a ");
		write(2, &ch, 1);
		putstr_fd(2, " ;\n");
		return (0);
	}
	return (1);
}

int	handle_quote(char *str, int i, int ch)
{
	int		count;
	char	*tmp;
	int		tmp_i;

	count = 2;
	i++;
	tmp_i = i;
	tmp = ft_strchr_premium(str, (str + i), ch, &tmp_i);
	while (str[i] != tmp[0] || (str[i] == tmp[0] && is_escaped(str, i)))
	{
		i++;
		count++;
	}
	return (count);
}
