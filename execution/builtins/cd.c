#include "../execution.h"

static char	*cd_home_error(char *oldpwd)
{
	free(oldpwd);
	ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	g_exit_status = 1;
	return (NULL);
}

static char	*cd_oldpwd_error(char *oldpwd)
{
	free(oldpwd);
	ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
	g_exit_status = 1;
	return (NULL);
}

static char	*get_cd_path(char **args, t_env **env, char *oldpwd)
{
	char	*path;

	if (!args[1] || !ft_strcmp(args[1], "~"))
	{
		path = get_env_value(*env, "HOME");
		if (!path)
			return (cd_home_error(oldpwd));
	}
	else if (!ft_strcmp(args[1], "-"))
	{
		path = get_env_value(*env, "OLDPWD");
		if (!path)
			return (cd_oldpwd_error(oldpwd));
	}
	else
		path = args[1];
	return (path);
}

int			ft_cd(char **args, t_env **env)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;
	int		print_newpwd;

	oldpwd = getcwd(NULL, 0);
	print_newpwd = (args[1] && !ft_strcmp(args[1], "-"));
	path = get_cd_path(args, env, oldpwd);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		free(oldpwd);
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_exit_status = 1;
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	update_env_var(env, "OLDPWD", oldpwd);
	update_env_var(env, "PWD", newpwd);
	if (print_newpwd && newpwd)
		printf("%s\n", newpwd);
	free(oldpwd);
	free(newpwd);
	g_exit_status = 0;
	return (0);
}
