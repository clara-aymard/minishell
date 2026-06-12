/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 11:16:50 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 13:33:57 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	ft_bzero(void *str, size_t n)
{
	unsigned long int	i;

	if (n == 0)
		return ;
	i = 0;
	while (i < n)
	{
		*(char *) str = '\0';
		str++;
		i++;
	}
}

size_t	ft_strlcat(char *dst, char *src, size_t dstsize)
{
	size_t	dstlen;
	size_t	srclen;
	size_t	i;
	size_t	j;

	srclen = ft_strlen(src);
	if (!dst && dstsize == 0)
		return (srclen);
	dstlen = ft_strlen(dst);
	if (dstsize <= dstlen || dstsize == 0)
		return (srclen + dstsize);
	i = dstlen;
	j = 0;
	while (src[j] != '\0' && j < dstsize - dstlen - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (dstlen + srclen);
}

char	*ft_strnstr(char *haystack, char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	z;

	if (!*needle)
		return ((char *) haystack);
	i = 0;
	while (haystack[i] != '\0')
	{
		j = 0;
		if (haystack[i] == needle[j])
		{
			z = i;
			while (i < len && haystack[z] == needle[j] && needle[j] != '\0')
			{
				j++;
				z++;
				if (z <= len && needle[j] == '\0')
					return ((char *)haystack + i);
			}
		}
		i++;
	}
	return (NULL);
}

char	*ft_substr(char *str, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;

	if (!str)
		return (NULL);
	if (start >= (unsigned int)ft_strlen(str))
		ptr = (char *) malloc(sizeof(char));
	else if (len < (unsigned int)ft_strlen(str) - start)
		ptr = (char *) malloc(len + 1 * sizeof(char));
	else
		ptr = (char *) malloc((((unsigned int)ft_strlen(str) - start) + 1)
				* sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len && start < (unsigned int)ft_strlen(str))
	{
		ptr[i] = str[start];
		start++;
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}
