/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:43:28 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/21 15:21:46 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

char	*get_delquote(t_expand_tools *exp_tools, char *s)
{
	int	i;

	i = -1;
	exp_tools->in_dquote = 0;
	exp_tools->in_squote = 0;
	while (s[++i])
	{
		check_quotes(exp_tools, s, &i, 0);
		if (s[i] == '"' && !is_escaped(s, i) && !exp_tools->in_squote)
			exp_tools->nb_delquote++;
		else if (s[i] == '\'' && !is_escaped(s, i) && !exp_tools->in_dquote)
			exp_tools->nb_delquote++;
	}
	return (s);
}

void	get_delbslash(t_expand_tools *exp_tools, char *s)
{
	int		i;

	i = -1;
	while (s[++i])
	{
		check_quotes(exp_tools, s, &i, 0);
		if (exp_tools->in_squote)
			continue ;
		if (!exp_tools->in_dquote && (s[i] == '\\'
				&& s[i + 1] && !is_escaped(s, i)))
		{
			i++;
			exp_tools->nb_delbslash++;
		}
		else if (exp_tools->in_dquote && (s[i] == '\\'
				&& s[i + 1]) && !is_escaped(s, i)
			&& (s[i + 1] == '\\' || s[i + 1] == '"'))
		{
			i++;
			exp_tools->nb_delbslash++;
		}
	}
}

char	*del_quote_bslash(t_mini *shell, t_expand_tools *e_tl, char *str)
{
	int		i;
	int		j;
	char	*new_str;

	if (!e_tl->nb_delbslash && !e_tl->nb_delquote)
		return (str);
	new_str = malloc((sizeof(char) * (ft_strlen(str) - e_tl->nb_word)) + 1);
	if (!new_str)
	{
		if (e_tl->is_var == 1)
			free(str);
		ft_error_parser(2, shell, NULL);
	}
	i = 0;
	j = 0;
	while (str[i])
	{
		check_quotes(e_tl, str, &i, 0);
		if (!cond_delquote_bslash(e_tl, str, i))
			new_str[j++] = str[i++];
		else
			i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	init_exp_tools(t_expand_tools *exp_tools, char *input)
{
	exp_tools->in_dquote = 0;
	exp_tools->in_squote = 0;
	exp_tools->input = input;
	exp_tools->tab_var = NULL;
	exp_tools->is_var = 0;
	exp_tools->nb_word = 0;
	exp_tools->is_quote = 0;
	exp_tools->nb_delbslash = 0;
	exp_tools->nb_delquote = 0;
}

void	expander_exe(t_lexer **lex_list, t_mini *shell)
{
	t_lexer			*tmp;
	char			*str_tmp;
	t_expand_tools	exp_tools;
	char			*str_expand;

	tmp = *(lex_list);
	while (tmp)
	{
		if (tmp->token == WORD && !(tmp->prev && tmp->prev->token == HERE_DOC))
		{
			init_exp_tools(&exp_tools, tmp->str);
			str_expand = var_expander(&exp_tools, shell);
			get_delbslash(&exp_tools, str_expand);
			get_delquote(&exp_tools, str_expand);
			exp_tools.nb_word = exp_tools.nb_delbslash + exp_tools.nb_delquote;
			str_tmp = del_quote_bslash(shell, &exp_tools, str_expand);
			if (str_tmp != str_expand && str_expand != tmp->str)
				free(str_expand);
			if (str_tmp != tmp->str)
				free(tmp->str);
			tmp->str = str_tmp;
		}
		tmp = tmp->next;
	}
}
