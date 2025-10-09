#include "../includes/minishell.h"

int replace_single_variable(char *str, t_ctx *ctx, char *result, char **env)
{
    int start, k;
    char *var;
    char *val;

    start = ctx->i;
    while (str[ctx->i] && (ft_isalnum(str[ctx->i]) || str[ctx->i] == '_'))
        (ctx->i)++;
    var = ft_substr(str, start, ctx->i - start);
    val = get_env_value_array(var, env);
    k = 0;
    while (val && val[k])
        result[ctx->j++] = val[k++];
    free(var);
    free(val);
    return ctx->j;
}

int handle_dollars(char *str, t_ctx *ctx, char *result, char **env)
{
    int count, mod;

    count = 0;
    while (str[ctx->i + count] == '$')
        count++;
    mod = count / 2;
    while (mod--)
        ctx->j = double_dollars(result, ctx->j);
    if (count % 2 == 1)
    {
        ctx->i += count;
        if (str[ctx->i] && (ft_isalnum(str[ctx->i]) || str[ctx->i] == '_'))
            ctx->j = replace_single_variable(str, ctx, result, env);
        else
            result[ctx->j++] = '$';
    }
    else
        ctx->i += count;
    return ctx->j;
}

char *expand(t_token *tokens, char **env)
{
	char *var_val;

	var_val = NULL;
	var_val = replace_in_arg(tokens->value,env);
	return(var_val);
}

char *replace_in_arg(char *str, char **env)
{
    char    *result;
   	t_ctx     ctx;

    result = malloc(4096);
    if (!result)
        return NULL;
    ctx.i = 0;
    ctx.j = 0;
    while (str[ctx.i])
    {
        if (str[ctx.i] == '$')
            ctx.j = handle_dollars(str,&ctx, result, env);
        else
            result[ctx.j++] = str[ctx.i++];
    }
    result[ctx.j] = '\0';
    return result;
}

t_token *expand_token(t_token *token, char **envp)
{
    t_token *new;
    char *res;
    char *name = var_name(token->value);

    if (token_has_quotes(token))
        res = handle_double(token, envp);
    else
        res = expand(token, envp);

    if (token_has_quotes(token))
        new = create_token(QUOTED_VAR, res, name);
    else
        new = create_token(VAR, res, name);

    free(res);
    return new;
}

t_token *expand_variables(t_token *tokens, char **envp)
{
    t_token *result;
    t_token *new;
    t_token *prev;

    result = NULL;
    prev = NULL;
    while (tokens)
    {
        if (is_it_var(tokens->value) && 
			!(prev && prev->type == HERE_DOC))
            new = expand_token(tokens, envp);
        else
            new = create_token(tokens->type, tokens->value, NULL);

        append_list(&result, new);
        prev = tokens;
        tokens = tokens->next;
    }
    return result;
}

int split_var_token(t_token *xpnd, t_token **result)
{
    char **tmp;
    t_token *new;
    int i;

    tmp = ft_split(xpnd->value, ' ');
    if (!tmp)
        return 0;
    i = 0;
    while (tmp[i])
    {
        new = create_token(VAR, tmp[i], xpnd->var_nam);
        if (!new)
        {
            free_split(tmp);
            return -1;
        }
        append_list(result, new);
        i++;
    }
    free_split(tmp);
    return 1;
}

int copy_token(t_token *xpnd, t_token **result)
{
    t_token *new;

    new = create_token(xpnd->type, xpnd->value, xpnd->var_nam);
    if (!new)
        return 0;
    append_list(result, new);
    return 1;
}

int handle_token(t_token *xpnd, t_token **result, t_token *prev)
{
    if (xpnd->type == VAR && ft_strchr(xpnd->value, ' ') &&
        (prev == NULL || prev->type != HERE_DOC))
        return split_var_token(xpnd, result);
    else
        return copy_token(xpnd, result);
}

t_token *expanding_it(t_token *token, char **env)
{
    t_token *xpnd = expand_variables(token, env);
    t_token *result = NULL;
    t_token *prev = NULL;
    t_token *xpnd_head = xpnd;

    while (xpnd)
    {
        if (handle_token(xpnd, &result, prev) <= 0)
        {
            free_token_list(result);
            free_token_list(xpnd_head);
            return NULL;
        }
        prev = xpnd;
        xpnd = xpnd->next;
    }

    free_token_list(xpnd_head);
    return result;
}
