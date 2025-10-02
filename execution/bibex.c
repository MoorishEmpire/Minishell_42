#include "../includes/minishell.h"

static void	setup_pipe_redirection(t_child_ctx *ctx)
{
	if (ctx->i != 0 && ctx->pipes)
	{
		dup2(ctx->pipes[ctx->i - 1][0], STDIN_FILENO);
		close(ctx->pipes[ctx->i - 1][0]);
	}
	if (ctx->i != ctx->cmd_count - 1 && ctx->pipes)
	{
		dup2(ctx->pipes[ctx->i][1], STDOUT_FILENO);
		close(ctx->pipes[ctx->i][1]);
	}
}

static void	close_unused_pipes(t_child_ctx *ctx)
{
	int	i;

	if (!ctx->pipes)
		return ;
	i = 0;
	while (i < ctx->cmd_count - 1)
	{
		if (ctx->pipes[i])
		{
			if (i != ctx->i - 1)
				close(ctx->pipes[i][0]);
			if (i != ctx->i)
				close(ctx->pipes[i][1]);
		}
		i++;
	}
}

void	child_proc(t_child_ctx *ctx)
{
	g_exit_status = 0;
	set_signals_in_child();
	apply_redirection(ctx->cur);
	close_unused_pipes(ctx);
	setup_pipe_redirection(ctx);
	if (is_builtin(ctx->cur->argv[0]))
		exit(execute_builtin(ctx->cur, &ctx->env));
	exec_externals_in_child(ctx->cur, ctx->env);
	exit(EXIT_FAILURE);
}

void	execute_pipeline(t_cmd *cmds, t_env *env)
{
	t_pipes_ctx	p_ctx;

	if (!initialize_pipeline_resources(&p_ctx, cmds))
		return ;
	execute_pipeline_commands(&p_ctx, env, cmds);
	finalize_pipeline_execution(&p_ctx);
}
