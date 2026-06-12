/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:38:28 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/21 17:11:05 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static	int	read_word(char *str, int i, t_token *token)
{
	int	count;
	int	res;

	count = 0;
	res = 0;
	while (str[i] && !is_token(str, i) && !ft_is_white_space(str[i]))
	{
		if ((str[i] == '\'' || str[i] == '"') && !is_escaped(str, i))
		{
			res = handle_quote(str, i, str[i]);
			i += res;
			count += res;
		}
		else
		{
			count++;
			i++;
		}
	}
	*token = WORD;
	return (count);
}

void	lexer_exe(char *str, t_lexer **lex_list, t_mini *shell)
{
	int		i;
	int		cursor;
	t_token	token;
	char	*word;

	i = 0;
	cursor = 0;
	token = 0;
	while (str[i])
	{
		while (ft_is_white_space(str[i]) && str[i])
			i++;
		if (!str[i])
			return ;
		if (is_token(str, i))
			cursor += handle_token(str, i, &token);
		else
			cursor += read_word(str, i, &token);
		word = ft_substr(str, i, cursor);
		if (!word)
			ft_error_parser(2, shell, NULL);
		addnode_lexer(shell, lex_list, token, word);
		i += cursor;
		cursor = 0;
	}
}
