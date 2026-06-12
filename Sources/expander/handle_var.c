/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:39:37 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/22 13:53:34 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"


static char	*handle_var(char *var, t_envp **envp, t_mini *shell, char **tab_str)
{
	t_envp	*tmp;
	char	*str;
	int		len;

	if ((!ft_strncmp(var, "?", 1) && var[1] == '\0'))
	{
		str = ft_itoa(shell->status);
		if (!str || !str[0])
			ft_error_parser(2, shell, tab_str);
		return (str);
	}
	tmp = *(envp);
	while (tmp)
	{
		len = ft_strlen(var);
		if (len == (int)ft_strlen(tmp->name)
			&& ft_strnstr(tmp->name, var, len))
			break ;
		tmp = tmp->next;
	}
	if (tmp && tmp->value)
		str = ft_strdup(tmp->value);
	else
		str = ft_strdup("");
	return (str);
}

char	*expand_var(t_expand_tools *e_tl, int *i, t_mini *shell, char **tab_str)
{
	char	*expanded_var;
	char	*str;
	int		k;
	int		cursor;
	char	*substr;

	str = e_tl->input;
	k = *i + 1;
	cursor = 0;
	while (str[k] && !is_special_char(str, str[k], k))
	{
		cursor++;
		if (str[k] == '?')
			break ;
		k++;
	}
	substr = ft_substr(str, *i + 1, cursor);
	if (!substr)
		ft_error_parser(2, shell, tab_str);
	expanded_var = handle_var(substr, &shell->envp, shell, tab_str);
	if (!expanded_var)
		ft_error_parser(2, shell, tab_str);
	*i += (cursor + 1);
	free(substr);
	return (expanded_var);
}

static void	get_tab_var(t_expand_tools *e_tl, char *str, t_mini *shell)
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
		check_quotes(e_tl, str, &i, 0);
		if (condition_var(e_tl, str, i, 1))
			e_tl->tab_var[j++] = expand_var(e_tl, &i, shell, e_tl->tab_var);
		else
			e_tl->tab_var[j++] = get_word_var(e_tl, shell, str, &i);
	}
	e_tl->tab_var[j] = 0;
}

static void	get_nb_word(t_expand_tools *exp_tools, char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_strlen(str) > 1)
		var_exe_nbword(exp_tools, str, &i);
}

char	*var_expander(t_expand_tools *exp_tools, t_mini *shell)
{
	char	*expanded_str;

	get_nb_word(exp_tools, exp_tools->input);
	if (exp_tools->is_var == 0)
		return (exp_tools->input);
	get_tab_var(exp_tools, exp_tools->input, shell);
	expanded_str = conc_strings(shell, exp_tools->tab_var);
	free_table(exp_tools->tab_var);
	return (expanded_str);
}
