/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:38:39 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/15 14:09:44 by tamigore         ###   ########.fr       */
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

typedef enum s_type
{
	word,
	pip,
	rin,
	rdin,
	rout,
	rdout,
	fd,
	lim,
}	t_type;

typedef struct s_token
{
	int				type;
	char			*str;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	t_token			*arg;
	t_token			*redir;
	int				fdin;
	int				fdout;
	int				pid;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_cmd	*cmd;
}	t_shell;

/*
** free.c
*/

void	free_token(t_token *token);
void	free_cmd(t_cmd *cmd);
void	exit_free(void *ptr, char *err, char type);

/*
** usefull_pars.c
*/

void	skip_cot(char *str, int *i);
int 	check_sep(char c);
void	split(char *str, int *i, int *last);
void	get_type(t_token *tmp, int *l, int *f);

/*
** pars.c
*/

void    split_words(char *str);

/*
** init.c
*/

t_token	*init_token(t_token *next, char *str, int type);

/*
** print.c
*/

void	print_cmd(t_cmd *cmd);
void	print_token(t_token *token);

#endif