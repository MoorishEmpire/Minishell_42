#include "../execution.h"

static void	remove_env_var(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->var, key))
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->var);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int			ft_unset(char **args, t_env **env)
{
	int	status;
	int	i;

	status = 0;
	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (!is_valid_key(args[i]))
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			remove_env_var(env, args[i]);
		i++;
	}
	g_exit_status = status;
	return (status);
}
