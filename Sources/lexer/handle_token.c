/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatabou <tbatabou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:00:19 by tbatabou          #+#    #+#             */
/*   Updated: 2024/05/10 15:55:56 by tbatabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int	handle_token(char *str, int i, t_token *token)
{
	if (str[i] == '<')
	{
		if (str[i + 1] && str[i + 1] == '<')
			*token = HERE_DOC;
		else
			*token = INPUT;
	}
	else if (str[i] == '>')
	{
		if (str[i + 1] && str[i + 1] == '>')
			*token = APPEND;
		else
			*token = TRUNC;
	}
	else if (str[i] == '|')
		*token = PIPE;
	if (*token && (*token == APPEND || *token == HERE_DOC))
		return (2);
	else if (*token)
		return (1);
	else
		return (0);
}

int	is_token(char *str, int i)
{
	if (str[i] && str[i] == '<' && !is_escaped(str, i))
		return (1);
	else if (str[i] && str[i] == '>' && !is_escaped(str, i))
		return (1);
	else if (str[i] && str[i] == '|' && !is_escaped(str, i))
		return (1);
	else
		return (0);
}
