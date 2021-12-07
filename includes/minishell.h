/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:38:39 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/07 17:23:01 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>

typedef struct s_pipe
{
	char			*cmd;
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
	char			*args;
	char			*cmd;
	char			*op;
}	t_pars;

#endif