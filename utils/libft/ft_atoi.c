/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-most <ael-most@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:34:42 by ael-most          #+#    #+#             */
/*   Updated: 2024/11/07 15:34:20 by ael-most         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int					i;
	unsigned long long	number;
	int					sign;

	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	number = 0;
	while (ft_isdigit(str[i]))
	{
		number = number * 10 + str[i] - '0';
		if (number > LLONG_MAX && sign == 1)
			return (-1);
		if (number > LLONG_MAX && sign == -1)
			return (0);
		i++;
	}
	return ((int)sign * number);
}
