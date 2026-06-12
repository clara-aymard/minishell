/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi_itoa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:06:45 by caymard           #+#    #+#             */
/*   Updated: 2024/05/06 14:05:09 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

long	ft_atoi(char *str)
{
	int		i;
	int		sign;
	long	nbr;
	long	a;

	i = 0;
	sign = 1;
	nbr = 0;
	if (!str)
		return (-2147483649);
	while (((9 <= str[i]) && (str[i] <= 13)) || (str[i] == 32))
		i++;
	if (str[i] == '-')
		sign = -1;
	if ((str[i] == '-') || (str[i] == '+'))
		i++;
	while (('0' <= str[i]) && (str[i] <= '9'))
	{
		a = nbr;
		nbr = (nbr * 10) + (str[i] - '0');
		i++;
		if (nbr < a)
			return (-2147483649);
	}
	return (nbr * sign);
}

static int	len_n(int n)
{
	int	i;

	if (!n)
		return (1);
	i = 0;
	if (n < 0)
	{
		n = -n;
		i++;
	}
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	i = len_n(n) - 1;
	str = malloc(sizeof(char) * (i + 2));
	if (!str)
		return (0);
	str[i + 1] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		n = -n;
	}
	while (9 < n)
	{
		str[i] = (n % 10) + '0';
		n = n / 10;
		i--;
	}
	str[i] = n + '0';
	return (str);
}
