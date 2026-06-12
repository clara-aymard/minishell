/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:25:54 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/17 15:33:40 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

char	*hd_get_word_var(t_mini *sh, t_expand_tools *e_tl, char *str, int *i)
{
	int		cursor;
	char	*word;

	cursor = *i;
	while (condition_var(e_tl, str, cursor, 3))
		cursor++;
	word = ft_substr(str, *i, cursor - *i);
	if (!word)
		ft_error_parser(2, sh, e_tl->tab_var);
	*i = cursor;
	return (word);
}

void	hd_get_tab_var(t_expand_tools *e_tl, char *str, t_mini *shell)
{
	int		i;
	int		j;

	e_tl->tab_var = (char **)malloc(sizeof(char *) * (e_tl->nb_word + 1));
	if (!e_tl->tab_var)
		ft_error_parser(2, shell, NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (condition_var(e_tl, str, i, 1))
			e_tl->tab_var[j++] = expand_var(e_tl, &i, shell, e_tl->tab_var);
		else
			e_tl->tab_var[j++] = hd_get_word_var(shell, e_tl, str, &i);
	}
	e_tl->tab_var[j] = 0;
}

static void	hd_var_exe_nbword(t_expand_tools *exp_tools, char *str, int *i)
{
	int	cursor;

	cursor = 0;
	if (condition_var(exp_tools, str, *i, 1))
	{
		exp_tools->is_var = 1;
		cursor = *i + 1;
		while (condition_var(exp_tools, str, cursor, 2))
			cursor++;
		exp_tools->nb_word++;
		*i = cursor;
	}
	else
	{
		cursor = *i;
		while (condition_var(exp_tools, str, cursor, 3))
			cursor++;
		exp_tools->nb_word++;
		*i = cursor;
	}
}

static void	hd_get_nb_word(t_expand_tools *exp_tools, char *str)
{
	int	i;

	i = 0;
	while (str[i])
		hd_var_exe_nbword(exp_tools, str, &i);
}

char	*hd_var_expander(t_mini *shell, char *str)
{
	char			*expanded_str;
	t_expand_tools	exp_tools;

	init_exp_tools(&exp_tools, str);
	hd_get_nb_word(&exp_tools, exp_tools.input);
	if (exp_tools.is_var == 0)
		return (str);
	hd_get_tab_var(&exp_tools, str, shell);
	free(str);
	expanded_str = conc_strings(shell, exp_tools.tab_var);
	free_table(exp_tools.tab_var);
	exp_tools.tab_var = NULL;
	return (expanded_str);
}
