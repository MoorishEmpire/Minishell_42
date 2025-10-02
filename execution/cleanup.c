#include "../execution.h"

void	clear_tokens(t_token **head)
{
	t_token	*tmp;

	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		free(tmp->value);
		if (tmp->var_nam)
			free(tmp->var_nam);
		free(tmp);
	}
}

void	clear_cmd(t_cmd **head)
{
	t_cmd	*tmp;
	int		i;

	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		if (tmp->argv)
		{
			i = 0;
			while (tmp->argv[i])
				free(tmp->argv[i++]);
			free(tmp->argv);
		}
		if (tmp->redirect)
		{
			i = 0;
			while (tmp->redirect[i])
				free(tmp->redirect[i++]);
			free(tmp->redirect);
		}
		if (tmp->file)
		{
			i = 0;
			while (tmp->file[i])
				free(tmp->file[i++]);
			free(tmp->file);
		}
		free(tmp);
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->var);
		free(tmp->value);
		free(tmp);
	}
}

void	free_2d_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
