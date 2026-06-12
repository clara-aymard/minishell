/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:23:12 by caymard           #+#    #+#             */
/*   Updated: 2024/05/08 11:16:26 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	*free_split(char **word, int x)
{
	int	i;

	i = 0;
	while (i < x)
	{
		free(word[i]);
		i++;
	}
	free(word);
	return (0);
}

static int	ft_count_word(char const *str, char c)
{
	size_t	i;
	int		word;
	int		a;

	i = 0;
	word = 0;
	a = 0;
	while (str[i])
	{
		if ((str[i] != c) && (a == 0))
		{
			a = 1;
			word++;
		}
		else if (str[i] == c)
			a = 0;
		i++;
	}
	return (word);
}

static void	ft_write_word(char *dest, const char *from, char c)
{
	size_t	i;

	i = 0;
	while (from[i] && (from[i] != c))
	{
		dest[i] = from[i];
		i++;
	}
	dest[i] = '\0';
}

static char	**ft_write_split(char **word, char const *str, char c)
{
	size_t	i;
	size_t	j;
	size_t	x;

	i = 0;
	x = 0;
	while (str[i])
	{
		if (str[i] == c)
			i++;
		else
		{
			j = 0;
			while ((str[i + j] != c) && str[i + j])
				j++;
			word[x] = (char *)malloc(sizeof(char) * (j + 1));
			if (!word[x])
				return ((char **)free_split(word, x));
			ft_write_word(word[x], &str[i], c);
			i = i + j;
			x++;
		}
	}
	return (word);
}

char	**split(char const *str, char c)
{
	char	**tab;
	int		len;

	if (!str)
		return (0);
	len = ft_count_word(str, c);
	tab = (char **)malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (0);
	tab[len] = NULL;
	return (ft_write_split(tab, str, c));
}
