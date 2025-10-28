#include "../includes/minishell.h"

void	close_pipes(int **pipe, int count)
{
	int	i;

	if (!pipe)
		return ;
	i = 0;
	while (i < count)
	{
		if (pipe[i])
		{
			close(pipe[i][0]);
			close(pipe[i][1]);
		}
		i++;
	}
}

int	initialize_pipeline_resources(t_pipes_ctx *p_ctx, t_cmd *cmds)
{
	p_ctx->cmd_count = count_cmd(cmds);
	if (p_ctx->cmd_count <= 0)
		return (0);
	p_ctx->pids = malloc(sizeof(pid_t) * p_ctx->cmd_count);
	if (!p_ctx->pids)
	{
		g_exit_status = 1;
		return (0);
	}
	p_ctx->pipes = NULL;
	return (1);
}

void	cleanup_pipeline_resources(t_pipes_ctx *p_ctx)
{
	if (p_ctx->pipes)
	{
		close_pipes(p_ctx->pipes, p_ctx->cmd_count - 1);
		free_pipes(p_ctx->pipes, p_ctx->cmd_count - 1);
		p_ctx->pipes = NULL;
	}
	if (p_ctx->pids)
	{
		free(p_ctx->pids);
		p_ctx->pids = NULL;
	}
}