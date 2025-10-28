/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-most <ael-most@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:10:33 by ael-most          #+#    #+#             */
/*   Updated: 2024/11/03 22:50:25 by ael-most         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	unsigned int	i;
	size_t			n_len;

	if (*needle == '\0')
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	n_len = ft_strlen(needle);
	i = 0;
	while (haystack[i] && i + n_len <= len)
	{
		if (haystack[i] == needle[0])
		{
			if (ft_strncmp(&haystack[i], needle, n_len) == 0)
				return ((char *)&haystack[i]);
		}
		i++;
	}
	return (NULL);
}
