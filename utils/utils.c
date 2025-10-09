#include "../includes/minishell.h"

void	free_split(char **arr)
{
	int i;

	if (!arr)
		return;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
