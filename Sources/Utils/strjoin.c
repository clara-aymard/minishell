/* ************************************************************************** */
/*			                                                                */
/*                                                        :::      ::::::::   */
/*   strjoin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:23:39 by caymard           #+#    #+#             */
/*   Updated: 2024/04/04 20:23:40 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	if (!dest)
		return (src);
	if (!src)
		return (dest);
	j = 0;
	i = ft_strlen(dest);
	while (src[j])
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*strjoin(char *str1, char *str2, char *c)
{
	char	*new;

	if (c)
	{
		new = malloc(ft_strlen(str1) + ft_strlen(str2) + 1 + 1);
		if (!new)
			return (error_msg(0, strerror(errno), NULL, NULL), NULL);
		new[0] = '\0';
		new = ft_strcat(new, str1);
		new = ft_strcat(new, c);
		new = ft_strcat(new, str2);
	}
	else
	{
		new = malloc(ft_strlen(str1) + ft_strlen(str2) + 1);
		if (!new)
			return (error_msg(0, strerror(errno), NULL, NULL), NULL);
		new[0] = '\0';
		new = ft_strcat(new, str1);
		new = ft_strcat(new, str2);
	}
	return (new);
}
