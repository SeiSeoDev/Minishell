/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:38:39 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/08 16:18:16 by tamigore         ###   ########.fr       */
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
	char			*cmd;
	char			*oper;
	char			*opt;
	struct s_redir	*redir;
}	t_arg;

typedef struct s_pipe
{
	struct s_arg	*arg;
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
	struct s_arg	*arg;
	struct s_pipe	*pipe;
}	t_pars;

typedef struct s_shell
{
	struct s_pars *pars;
}	t_shell;

/*
** free.c
*/

void	free_arg(t_arg *arg);
void	exit_free(t_pars *pars, char *err);

/*
** check.c
*/

int		check_line(char *str);

/*
** get.c
*/

int		get_cmd(char *res, char *str);
int		get_oper(char *res, char *str, int i);

/*
** init.c
*/

void	init_pipe(t_pars *pars);
void	init_arg(t_pars *pars);

#endif