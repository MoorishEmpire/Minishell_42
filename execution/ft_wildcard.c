#include "../../includes/minishell.h"

static int	match_pattern(const char *pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *str == '\0')
		return (0);
	if (*pattern == '*')
		return (match_pattern(pattern + 1, str) || match_pattern(pattern, str
				+ 1));
	if (*pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

static void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(matches[i], matches[j]) > 0)
			{
				temp = matches[i];
				matches[i] = matches[j];
				matches[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static char	**get_matches(t_token *current, int *count, int max_matches)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = (char **)malloc(sizeof(char *) * max_matches);
	if (!matches)
	{
		closedir(dir);
		return (NULL);
	}
	*count = 0;
	entry = readdir(dir);
	while (entry != NULL && *count < max_matches)
	{
		if (entry->d_name[0] != '.' || current->value[0] == '.')
			if (match_pattern(current->value, entry->d_name))
				matches[(*count)++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

static void	add_matches_to_tokens(t_token **tokens, t_token **last,
		char **matches, int count)
{
	int	i;

	if (count > 0)
	{
		sort_matches(matches, count);
		add_token(tokens, last, TOKEN_WORD, matches[0]);
		free(matches[0]);
		i = 1;
		while (i < count)
		{
			add_token(tokens, last, TOKEN_SPACE, " ");
			add_token(tokens, last, TOKEN_WORD, matches[i]);
			free(matches[i]);
			i++;
		}
	}
}

void	expand_wildcard(t_token **tokens, t_token **last, t_token *current)
{
	char	**matches;
	int		count;
	int		max_matches;

	max_matches = 100;
	matches = get_matches(current, &count, max_matches);
	if (!matches)
		return ;
	add_matches_to_tokens(tokens, last, matches, count);
	if (count == 0)
		add_token(tokens, last, TOKEN_WORD, current->value);
	free(matches);
}