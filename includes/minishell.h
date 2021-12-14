/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:38:39 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/14 14:43:34 by tamigore         ###   ########.fr       */
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
void	exit_free(t_cmd *cmd, char *err);

/*
** check.c
*/

int		check_line(char *str);

/*
** get.c
*/

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