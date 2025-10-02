#include "../includes/minishell.h"

int replace_variable(char *str, t_ctx *ctx, char *result, char **env)
{
	char	*var;
	char	*val;
	int		start;
	int		k;

	start = ++(ctx->i);
	while (str[ctx->i] && (ft_isalnum(str[ctx->i]) || str[ctx->i] == '_'))
		(ctx->i)++;
	var = ft_substr(str, start, ctx->i - start);
	val = get_env_value_array(var, env);
	k = 0;
	while (val && val[k])
		result[ctx->j++] = val[k++];
	free(var);
	free(val);
	return (ctx->j);
}

int handle_quotes(char *str, char *result, t_ctx *ctx)
{
    if (str[ctx->i] == '\'' && !ctx->in_double)
    {
        ctx->in_single = !ctx->in_single;
        result[ctx->j++] = str[ctx->i++];
        return (1);
    }
    if (str[ctx->i] == '"' && !ctx->in_single)
    {
        ctx->in_double = !ctx->in_double;
        result[ctx->j++] = str[ctx->i++];
        return (1);
    }
    return (0);
}

int handle_expansion(char *str, char *result, t_ctx *ctx, char **env)
{
    if (str[ctx->i] == '$' && str[ctx->i + 1] == '$' && !ctx->in_single)
    {
        ctx->j = double_dollars(result, ctx->j);
        ctx->i += 2;
        return (1);
    }
    if (str[ctx->i] == '$' && str[ctx->i + 1] == '"' && !ctx->in_single)
    {
        ctx->i++;
        return (1);
    }
    if (str[ctx->i] == '$' && !ctx->in_single &&
        (ft_isalnum(str[ctx->i + 1]) || str[ctx->i + 1] == '_'))
    {
        ctx->j = replace_variable(str, ctx, result, env);
        return (1);
    }
    return (0);
}

char *replace_in_quotes(char *str, char **env)
{
    char *result = malloc(4096);
	t_ctx ctx;

	ctx.i = 0;
	ctx.j = 0;
	ctx.in_single = 0;
	ctx.in_double = 0;
    if (!result)
        return NULL;
    while (str[ctx.i])
    {
        if (handle_quotes(str, result, &ctx))
            continue;

        if (handle_expansion(str, result, &ctx, env))
            continue;
        result[ctx.j++] = str[ctx.i++];
    }
    result[ctx.j] = '\0';
    return result;
}

char *handle_double(t_token *token, char **env)
{
    char *expanded;
    char *final_result;
    
    expanded = replace_in_quotes(token->value, env);
    if (!expanded)
        return NULL;
    final_result = remove_quotes(expanded);
    free(expanded);
    
    return final_result;
}

/* Helper function to get env value from array format */
char *get_env_value_array(char *name, char **env)
{
	int i = 0;
	int len = ft_strlen(name);

	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
		{
			char *value_start = env[i] + len + 1;
			return ft_strdup(value_start);
		}
		i++;
	}
	return (NULL);
}