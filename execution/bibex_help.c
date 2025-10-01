#include "../../includes/minishell.h"

static int	**allocate_pipes_array_only(int cmd_count)
{
	int	**pipes;
	int	i;

	if (cmd_count <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
	{
		g_exit_status = 1;
		return (NULL);
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = NULL;
		i++;
	}
	return (pipes);
}

static int	create_pipe_if_needed(int **pipes, int i, int cmd_count)
{
	if (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) < 0)
		{
			ft_putstr_fd("minishell: pipe creation failed\n", 2);
			g_exit_status = 1;
			return (0);
		}
	}
	return (1);
}

static void	setup_child_context(t_child_ctx *ctx, t_pipes_ctx *p_ctx,
		t_env *env)
{
	ctx->cur = p_ctx->cur;
	ctx->env = env;
	ctx->pipes = p_ctx->pipes;
	ctx->i = p_ctx->i;
	ctx->cmd_count = p_ctx->cmd_count;
}

void	execute_pipeline_commands(t_pipes_ctx *p_ctx, t_env *env, t_cmd *cmds)
{
	t_child_ctx	ctx;

	p_ctx->pipes = allocate_pipes_array_only(p_ctx->cmd_count);
	if (p_ctx->cmd_count > 1 && !p_ctx->pipes)
		return ;
	p_ctx->cur = cmds;
	p_ctx->i = 0;
	set_signals_for_child_execution();
	while (p_ctx->cur && p_ctx->i < p_ctx->cmd_count)
	{
		if (!create_pipe_if_needed(p_ctx->pipes, p_ctx->i, p_ctx->cmd_count))
			break ;
		setup_child_context(&ctx, p_ctx, env);
		p_ctx->pids[p_ctx->i] = fork();
		if (p_ctx->pids[p_ctx->i] == 0)
			child_proc(&ctx);
		else if (p_ctx->pids[p_ctx->i] < 0)
		{
			ft_putstr_fd("minishell: fork failed\n", 2);
			g_exit_status = 1;
			break ;
		}
		p_ctx->cur = p_ctx->cur->next;
		p_ctx->i++;
	}
}

void	finalize_pipeline_execution(t_pipes_ctx *p_ctx)
{
	int	save;

	save = g_exit_status;
	if (p_ctx->pipes)
		close_pipes(p_ctx->pipes, p_ctx->cmd_count - 1);
	if (p_ctx->i > 0)
		wait_all(p_ctx->pids, p_ctx->i);
	set_signals_interactive();
	cleanup_pipeline_resources(p_ctx);
	if (save == 1)
		g_exit_status = 1;
}