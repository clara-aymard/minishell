/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 13:30:50 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/22 11:50:24 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	minishell_exe(t_mini *shell)
{
	int	count_parser;

	expander_exe(&shell->lex_list, shell);
	parser_exe(&shell->lex_list, &shell->exec, shell, &count_parser);
	delete_lexlist(&shell->lex_list);
	sort_execution(shell, shell->exec);
}

char	*ft_strchr_premium(char *o_str, const char *p_str, int c, int *i)
{
	char	ch;

	ch = (char) c;
	(*i)++;
	while (*p_str != ch || (*p_str == ch && is_escaped(o_str, *i)))
	{
		if (*p_str == '\0' && ch == '\0')
			return ((char *) p_str);
		else if (*p_str == '\0')
			return (0);
		p_str++;
		(*i)++;
	}
	return ((char *) p_str);
}

char	*conc_strings(t_mini *shell, char **tab_strings)
{
	int		total_length;
	char	*result;
	int		i;

	total_length = 0;
	i = -1;
	while (tab_strings[++i] != 0)
		total_length += (int)ft_strlen(tab_strings[i]);
	result = (char *)malloc(sizeof(char) * (total_length + 1));
	if (!result)
		ft_error_parser(2, shell, tab_strings);
	result[0] = '\0';
	i = -1;
	while (tab_strings[++i] != 0)
		ft_strlcat(result, tab_strings[i], total_length + 1);
	return (result);
}

int	is_redirect_token(t_token token)
{
	if (token == INPUT || token == HERE_DOC
		|| token == APPEND || token == TRUNC)
		return (1);
	return (0);
}

int	is_special_char(char *str, char ch, int i)
{
	if (ch == '?' && str[i - 1] != '$')
		return (1);
	if (ch == '_' || ch == '?')
		return (0);
	if (is_alphanum(ch))
		return (0);
	return (1);
}
