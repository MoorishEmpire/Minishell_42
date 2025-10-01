#include "../execution.h"

t_env		*env_to_list(char **env)
{
	t_env	*head;
	t_env	*node;
	char	*equal;

	head = NULL;
	while (env && *env)
	{
		node = malloc(sizeof(t_env));
		if (!node)
			return (NULL);
		equal = ft_strchr(*env, '=');
		if (!equal)
		{
			free(node);
			env++;
			continue ;
		}
		node->var = ft_substr(*env, 0, equal - *env);
		node->value = ft_strdup(equal + 1);
		node->next = head;
		head = node;
		env++;
	}
	return (head);
}

char		**list_to_env(t_env *list)
{
	char	**env;
	char	*entry;
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = list;
	while (tmp)
	{
		if (tmp->var)
			count++;
		tmp = tmp->next;
	}
	env = malloc((count + 1) * sizeof(char *));
	if (!env)
		return (NULL);
	count = 0;
	while (list)
	{
		if (list->var)
		{
			entry = ft_strjoin(list->var, "=");
			env[count++] = ft_strjoin(entry, list->value ? list->value : "");
			free(entry);
		}
		list = list->next;
	}
	env[count] = NULL;
	return (env);
}

char		*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->var, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void		update_env_var(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;
	t_env	*new_node;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->var, key) == 0)
		{
			free(tmp->value);
			if (value)
				tmp->value = ft_strdup(value);
			else
				tmp->value = NULL;
			return ;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->var = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = *env;
	*env = new_node;
}

int			ft_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->var, env->value);
		env = env->next;
	}
	return (0);
}
