#include "../includes/minishell.h"

/* String comparison functions */
int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i = 0;
    
    if (n == 0)
        return (0);
    while (i < n - 1 && s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/* String duplication and manipulation */
char *ft_strdup(const char *s)
{
    char *dup;
    size_t len = ft_strlen(s);
    size_t i = 0;
    
    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}

char *ft_strjoin(const char *s1, const char *s2)
{
    char *joined;
    size_t len1, len2;
    size_t i = 0, j = 0;
    
    if (!s1 || !s2)
        return (NULL);
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    joined = malloc(len1 + len2 + 1);
    if (!joined)
        return (NULL);
    while (i < len1)
    {
        joined[i] = s1[i];
        i++;
    }
    while (j < len2)
    {
        joined[i + j] = s2[j];
        j++;
    }
    joined[i + j] = '\0';
    return (joined);
}

char *ft_substr(const char *s, unsigned int start, size_t len)
{
    char *substr;
    size_t i = 0;
    size_t s_len;
    
    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    if (len > s_len - start)
        len = s_len - start;
    substr = malloc(len + 1);
    if (!substr)
        return (NULL);
    while (i < len)
    {
        substr[i] = s[start + i];
        i++;
    }
    substr[i] = '\0';
    return (substr);
}

char *ft_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return ((char *)s);
        s++;
    }
    if ((char)c == '\0')
        return ((char *)s);
    return (NULL);
}

/* Output functions */
void ft_putstr_fd(const char *s, int fd)
{
    if (s)
        write(fd, s, ft_strlen(s));
}

/* Character checking functions */
int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int ft_isalpha(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int ft_isalnum(int c)
{
    return (ft_isalpha(c) || ft_isdigit(c));
}

/* String utilities */
size_t ft_strlen(const char *s)
{
    size_t len = 0;
    
    if (!s)
        return (0);
    while (s[len])
        len++;
    return (len);
}

/* Split function */
static int count_words(const char *s, char c)
{
    int count = 0;
    int in_word = 0;
    
    while (*s)
    {
        if (*s != c && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (*s == c)
            in_word = 0;
        s++;
    }
    return (count);
}

static char *get_word(const char *s, char c, int *len)
{
    char *word;
    int i = 0;
    
    *len = 0;
    while (s[*len] && s[*len] != c)
        (*len)++;
    word = malloc(*len + 1);
    if (!word)
        return (NULL);
    while (i < *len)
    {
        word[i] = s[i];
        i++;
    }
    word[i] = '\0';
    return (word);
}

char **ft_split(const char *s, char c)
{
    char **split;
    int words;
    int i = 0;
    int len;
    
    if (!s)
        return (NULL);
    words = count_words(s, c);
    split = malloc(sizeof(char *) * (words + 1));
    if (!split)
        return (NULL);
    while (*s && i < words)
    {
        while (*s && *s == c)
            s++;
        if (*s)
        {
            split[i] = get_word(s, c, &len);
            if (!split[i])
            {
                free_2d_arr(split);
                return (NULL);
            }
            s += len;
            i++;
        }
    }
    split[i] = NULL;
    return (split);
}

/* Integer to string conversion */
static int get_num_len(int n)
{
    int len = 0;
    
    if (n <= 0)
        len = 1;
    while (n)
    {
        n /= 10;
        len++;
    }
    return (len);
}

char *ft_itoa(int n)
{
    char *str;
    int len;
    long num;
    
    num = n;
    len = get_num_len(n);
    str = malloc(len + 1);
    if (!str)
        return (NULL);
    str[len] = '\0';
    if (num < 0)
    {
        str[0] = '-';
        num = -num;
    }
    if (num == 0)
        str[0] = '0';
    while (num > 0)
    {
        str[--len] = (num % 10) + '0';
        num /= 10;
    }
    return (str);
}

/* Array utilities */
void free_2d_arr(char **arr)
{
    int i = 0;
    
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}