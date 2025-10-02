#include "../includes/minishell.h"

char * unquoted(t_ctx *ctx, char *value)
{
	char *result;

	result = malloc(ctx->len + 1);
	if (!result)
        return NULL;
	while (ctx->i < ctx->len)
    {
        if (value[ctx->i] == '\'' && !ctx->in_double)
        {
            ctx->in_single = !ctx->in_single;
            ctx->i++;
        }
        else if (value[ctx->i] == '"' && !ctx->in_single)
        {
            ctx->in_double = !ctx->in_double;
            ctx->i++;
        }
        else
            result[ctx->j++] = value[ctx->i++];
    }
	result[ctx->j] = '\0';
	return(result);
}

char *remove_quotes(char *value)
{
	char *result;
	t_ctx ctx;

	ctx.i = 0;
	ctx.j = 0;
	ctx.in_single = 0;
	ctx.in_double = 0;
    if (!value)
        return NULL;
    ctx.len = ft_strlen(value);
	result = unquoted(&ctx,value);
    return (result);
}

char	*strip_token(char *value)
{
	int		i;
	int		quote_pos;

	quote_pos = -1;
	i = 0;
	while (value[i] != '\0')
	{
		if (value[i] == '"')
		{
			quote_pos = i;
			break;
		}
		i++;
	}
	if (quote_pos == -1)
		return (ft_strdup(value));
	return (remove_quotes(value));
}

int double_dollars(char *result, int j)
{
    pid_t pid;
    char *res;
    int l;

    pid = getpid();
    res = ft_itoa((int)pid);
    l = 0;
    while (res[l])
        result[j++] = res[l++];
    free(res);
    return (j);
}