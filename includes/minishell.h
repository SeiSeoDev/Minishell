/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:38:39 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/07 18:21:20 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"

typedef struct s_arg
{
	char			*str;
	char			**split;
	char			*cmd;
	char			*oper;
	char			*opt;
	char			*word;
	struct s_redir	*redir;
}	t_arg;


typedef struct s_pipe
{
	struct s_arg	arg;
	struct s_pipe	*prev;
	struct s_pipe	*next;
}	t_pipe;

typedef struct s_redir
{
	char	*out1;
	char	*out2;
	char	*in;
	int		stdout;
	int		stderr;
	int		stdin;
}	t_redir;

typedef struct s_pars
{
	char 			*line;
	char 			**split;
	struct s_pipe	*pipe;
	struct s_arg	*arg;
}	t_pars;

#endif