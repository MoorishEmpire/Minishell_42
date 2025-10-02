#include "../includes/minishell.h"

int	match_pattern(char *pattern, char *str)
{
	char	*star;
	char	*str_ptr;

	str_ptr = str;
	star = NULL;
	while (*str_ptr)
	{
		if (*pattern == '*')
		{
			star = pattern++;
			str = str_ptr;
		}
		else if (*pattern == *str_ptr || *pattern == '?')
		{
			pattern++;
			str_ptr++;
		}
		else if (star)
		{
			pattern = star + 1;
			str_ptr = ++str;
		}
		else
		{
			return (0);
		}
	}
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0');
}

int	matched_count(char *token)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.' && token[0] != '.')
			continue ;
		if (match_pattern(token, entry->d_name))
			count++;
	}
	closedir(dir);
	return (count);
}

int	no_matches(char *token)
{
	DIR				*dir;
	int				count;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.' && token[0] != '.')
			continue ;
		if (match_pattern(token, entry->d_name))
		{
			count++;
		}
	}
	closedir(dir);
	return (count);
}

static char	**init_matches_array(int total_count)
{
	char	**matches;

	matches = malloc((total_count + 1) * sizeof(char *));
	return (matches);
}

static char	**handle_no_expansion_case(char *token)
{
	char	**matches;

	matches = ft_split(token, '\0');
	return (matches);
}

static void	cleanup_matches_array(char **matches, int count)
{
	while (count-- > 0)
		free(matches[count]);
	free(matches);
}

static int	should_skip_entry(struct dirent *entry, char *token)
{
	if (entry->d_name[0] == '.' && token[0] != '.')
		return (1);
	return (0);
}

static int	add_match_to_array(char **matches, int count, char *filename)
{
	matches[count] = ft_strdup(filename);
	if (!matches[count])
		return (0);
	return (1);
}

static int	collect_directory_matches(char *token, char **matches)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (-1);
	while ((entry = readdir(dir)) != NULL)
	{
		if (should_skip_entry(entry, token))
			continue ;
		if (match_pattern(token, entry->d_name))
		{
			if (!add_match_to_array(matches, count, entry->d_name))
			{
				cleanup_matches_array(matches, count);
				closedir(dir);
				return (-1);
			}
			count++;
		}
	}
	closedir(dir);
	matches[count] = NULL;
	return (count);
}

char	**expand_wildcard_internal(char *token, NodeType type)
{
	char	**matches;
	int		total;
	int		result_count;

	total = matched_count(token);
	matches = init_matches_array(total);
	if (!matches)
		return (NULL);
	if (no_matches(token) == 0 || type == QUOTED_VAR)
		return (handle_no_expansion_case(token));
	result_count = collect_directory_matches(token, matches);
	if (result_count == -1)
	{
		free(matches);
		return (NULL);
	}
	return (matches);
}

static t_token	*make_list(t_token **result, int type, char *value,
		char *var_nam)
{
	t_token	*new;

	new = create_token(type, value, var_nam);
	if (!new)
	{
		free_token_list(*result);
		*result = NULL;
		return (NULL);
	}
	append_list(result, new);
	return (new);
}

static void	cleanup_expansion_results(char **res, int start_index)
{
	while (res[start_index])
		free(res[start_index++]);
	free(res);
}

static int	process_wildcard_token(t_token *token, t_token **result)
{
	char	**res;
	int		i;

	res = expand_wildcard_internal(token->value, token->type);
	if (!res)
		return (0);
	i = 0;
	while (res[i])
	{
		if (!make_list(result, token->type, res[i], token->var_nam))
		{
			cleanup_expansion_results(res, i);
			return (0);
		}
		free(res[i]);
		i++;
	}
	free(res);
	return (1);
}

static int	process_regular_token(t_token *token, t_token **result)
{
	return (make_list(result, token->type, token->value,
			token->var_nam) != NULL);
}

t_token	*handel_wild_card(t_token *xpnd)
{
	t_token	*result;

	result = NULL;
	while (xpnd)
	{
		if (ft_strchr(xpnd->value, '*'))
		{
			if (!process_wildcard_token(xpnd, &result))
				return (NULL);
		}
		else
		{
			if (!process_regular_token(xpnd, &result))
				return (NULL);
		}
		xpnd = xpnd->next;
	}
	return (result);
}