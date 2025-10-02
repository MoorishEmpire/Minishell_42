#ifndef MINISHELL_H
# define MINISHELL_H

/* Include both parsing and execution headers */
# include "includes.h"
# include "execution.h"

/* Color definitions for prompts */
# define PURPLE "\033[0;35m"
# define RESET  "\033[0m"

/* Additional utility functions that might be needed */

/* Print functions for debugging */
void	print_tokens(t_token *tokens);
void	print_parse(t_cmd *cmd);

/* Main execution flow */
void	process_line(char *input, char **env, t_env **env_list);

#endif
