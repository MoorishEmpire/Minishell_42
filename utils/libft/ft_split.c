/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-most <ael-most@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 18:45:22 by ael-most          #+#    #+#             */
/*   Updated: 2025/10/02 13:06:30 by ael-most         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s && *s != c)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static char	*ft_allocate(char const*s, char c)
{
	size_t	len;
	char	*new;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	new = ft_substr(s, 0, len);
	return (new);
}

static void	*ft_free(char **words, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(words[i++]);
	free(words);
	return (NULL);
}

static char	**ft_populate(char **words, char const *s, char c, int count_words)
{
	int	count;

	count = 0;
	while (*s && count < count_words)
	{
		while (*s == c)
			s++;
		if (*s && *s != c)
		{
			words[count] = ft_allocate(s, c);
			if (words[count] == NULL)
			{
				ft_free(words, count);
				return (NULL);
			}
			count++;
		}
		while (*s && *s != c)
			s++;
	}
	words[count] = NULL;
	return (words);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;
	int		count_words;

	if (!s)
		return (NULL);
	count_words = ft_count_words(s, c);
	ptr = malloc((count_words + 1) * sizeof(char *));
	if (ptr == NULL)
		return (NULL);
	ptr = ft_populate(ptr, s, c, count_words);
	return (ptr);
}
