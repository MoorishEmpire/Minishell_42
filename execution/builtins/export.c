#include "../execution.h"

static void	print_export(t_env *env)
{
	if (!env)
		return ;
	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->var, env->value);
		else
			printf("declare -x %s\n", env->var);
		env = env->next;
	}
}

int			is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha((unsigned char)key[i]) && key[i] != '_'))
		return (0);
	i++;
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum((unsigned char)key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	process_export_arg(char *arg, t_env **env)
{
	char	*eq;
	char	*key;
	char	*value;

	if (!is_valid_key(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		value = ft_strdup(eq + 1);
		update_env_var(env, key, value);
		free(key);
		free(value);
	}
	else
		update_env_var(env, arg, NULL);
	return (0);
}

int			ft_export(char **args, t_env **env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!args[1])
	{
		print_export(*env);
		return (0);
	}
	while (args[i])
	{
		if (process_export_arg(args[i], env))
			status = 1;
		i++;
	}
	g_exit_status = status;
	return (status);
}
