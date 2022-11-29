/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngobert <ngobert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 16:48:57 by nburat-d          #+#    #+#             */
/*   Updated: 2022/05/11 10:57:25 by ngobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

# define DIGIT "0123456789"
# define ALPHA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$-"
# define T_STRING "T_STRING"
# define T_PIPE "T_PIPE"
# define T_SQUOTE "T_SQUOTE"
# define T_DQUOTE "T_DQUOTE"
# define T_R_DIR "T_R_DIR"
# define T_L_DIR "T_L_DIR"
# define T_DR_DIR "T_DR_DIR"
# define T_DL_DIR "T_DL_DIR"
# define T_PLUS "T_PLUS"
# define T_MINUS "T_MINUS"
# define T_EQUAL "T_EQUAL"
# define T_MULTI "T_MULTI"
# define T_SLASH "T_SLASH"
# define T_PERCENT "T_PERCENT" 
# define T_RPAREN "T_RPAREN"
# define T_LPAREN "T_LPAREN"
# define T_RBRACK "T_RBRACK"
# define T_LBRACK "T_LBRACK"
# define T_DOLLAR "T_DOLLAR"
# define T_BUILTIN "T_BUILTIN"
# define STDIN "/dev/stdin"
# define STDOUT "/dev/stdout"

typedef struct s_token
{
	char	*content;
	char	*type;
	char	*quote;
	int		space_after;
}	t_token;

typedef struct s_lexer
{
	char	*text;
	int		pos;
	char	current_char;
}	t_lexer;

typedef struct s_tlist
{
	t_token			*token;
	struct s_tlist	*prev;
	struct s_tlist	*next;
}	t_tlist;

typedef struct s_cmd
{
	char			*command;
	char			**options;
	char			*type;
	bool			is_absolute;
	bool			is_double;
	bool			is_builtin;

	char			*bin;
	char			**infile;
	char			**outfile;
	char			**limiter;
	bool			update_o;
	bool			update_i;

	t_tlist			*builtin;

	int				fd_out;
	int				fd_in;

	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;

typedef struct s_minishell
{
	t_lexer	*lexer;
	t_dlist	*env;
	t_dlist	**head_env;
	t_cmd	*cmd;
	int		nb_cmd;
	int		err_exit;
}			t_minishell;

#endif