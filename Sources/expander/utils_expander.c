/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:26:44 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/21 12:21:14 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int	cond_delquote_bslash(t_expand_tools *exp_tools, char *str, int i)
{
	if (!exp_tools->in_squote && exp_tools->in_dquote
		&& (str[i] == '\\' && str[i + 1])
		&& (str[i + 1] == '\\' || str[i + 1] == '"')
		&& !is_escaped(str, i))
		return (1);
	else if ((!exp_tools->in_dquote) && !exp_tools->in_squote
		&& (str[i] == '\\' && str[i + 1] && !is_escaped(str, i)))
		return (1);
	else if (str[i] == '"' && !is_escaped(str, i) && !exp_tools->in_squote)
		return (1);
	else if (str[i] == '\'' && !is_escaped(str, i)
		&& !exp_tools->in_dquote)
		return (1);
	else
		return (0);
}

char	*get_word_var(t_expand_tools *e_tl, t_mini *shell, char *str, int *i)
{
	int		cursor;
	char	*word;

	cursor = *i;
	while (condition_var(e_tl, str, cursor, 3))
	{
		cursor++;
		check_quotes(e_tl, str, &cursor, 0);
	}
	word = ft_substr(str, *i, cursor - *i);
	if (!word)
		ft_error_parser(2, shell, e_tl->tab_var);
	*i = cursor;
	return (word);
}

void	var_exe_nbword(t_expand_tools *exp_tools, char *str, int *i)
{
	int	cursor;

	cursor = 0;
	check_quotes(exp_tools, str, i, 0);
	if (condition_var(exp_tools, str, *i, 1))
	{
		exp_tools->is_var = 1;
		cursor = *i + 1;
		while (condition_var(exp_tools, str, cursor, 2)
			&& str[cursor] && str[cursor] != '?')
			cursor++;
		exp_tools->nb_word++;
		*i = cursor;
	}
	else
	{
		cursor = *i;
		while (condition_var(exp_tools, str, cursor, 3) && str[cursor])
		{
			cursor++;
			check_quotes(exp_tools, str, &cursor, 0);
		}
		exp_tools->nb_word++;
		*i = cursor;
	}
}

int	condition_var(t_expand_tools *exp_tools, char *str, int i, int flag)
{
	if (flag == 1 && (str[i] == '$' && exp_tools->in_squote == 0)
		&& !is_escaped(str, i)
		&& (i < (int)ft_strlen(str) && str[i + 1]
			&& !is_special_char(str, str[i + 1], i + 1)))
		return (1);
	else if (flag == 2
		&& str[i]
		&& (!ft_is_white_space(str[i]) && !is_special_char(str, str[i], i)))
		return (1);
	else if (flag == 3
		&& str[i] && i < (int)ft_strlen(str)
		&& !(str[i] == '$' && !is_escaped(str, i)
			&& exp_tools->in_squote == 0
			&& !is_special_char(str, str[i + 1], i + 1)))
		return (1);
	return (0);
}

void	check_quotes(t_expand_tools *exp_tools, char *str, int *i, int flags)
{
	if (str[*i] == '\'' && !is_escaped(str, *i)
		&& exp_tools->in_squote == 1 && exp_tools->in_dquote == 0)
		exp_tools->in_squote = 0;
	else if (str[*i] == '\''
		&& !is_escaped(str, *i) && exp_tools->in_dquote == 0)
		exp_tools->in_squote = 1;
	if (str[*i] == '"' && !is_escaped(str, *i)
		&& exp_tools->in_dquote == 1 && exp_tools->in_squote == 0)
	{
		exp_tools->in_dquote = 0;
		if (flags)
			(*i)++;
	}
	else if (str[*i] == '"'
		&& !is_escaped(str, *i) && exp_tools->in_squote == 0)
	{
		exp_tools->in_dquote = 1;
		if (flags)
			(*i)++;
	}
}
