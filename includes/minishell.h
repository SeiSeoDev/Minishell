/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:38:39 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/26 16:42:03 by user42           ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>

#define STDIN		0
#define STDOUT		1
#define READ_END	0
#define WRTIE_END	1

typedef enum s_type
{
	word, // "whatever"
	pip, // |
	rin, // <
	rdin, // <<
	rout, // >
	rdout, // >>
	fd,	// >> "this"
	lim, // << "this"
}	t_type;

typedef struct s_token
{
	int				type; // t_type
	char			*str; // "str"
	int				fd;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	t_token			*arg; // while arg exec(arg) arg = arg->next
	t_token			*redir; // after > | < | >> 
	int				fdin; // a remplir avec l entree
	int				fdout; // ou est ce que j ecris/transmet
	int				pid;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*name;
	char			*val;
	struct s_env	*next;
}	t_env;

/*
** free.c
*/

void	free_token(t_token *token);
void	free_cmd(t_cmd *cmd);
void	free_env(t_env *env);
void	exit_free(void *ptr, char *err, char type);
void	free_tab(char **tab);

/*
** usefull_pars.c
*/

void	skip_cot(char *str, int *i);
void	split(char *str, int *i, int *last);
void	get_type(t_token *tmp, int *l, int *f);
int		quot_status(char *str, int i);

/*
** expend.c
*/

char	*del_unused_quot(char *str);
char	*expend_words(t_token *token, char *str);
t_token *cmd_arg(t_token **tmp);
t_token *cmd_redir(t_token **tmp);

/*
** pars.c
*/

void	split_words(char *str);

/*
** init.c
*/

t_env	*init_env(t_env *next, char *name, char *val);
t_token *init_token(t_token *next, char *str, int type);
t_cmd	*init_cmd(t_cmd *next, t_token *arg, t_token *redir);
void	add_token(t_token *token, t_token *new);
void	del_token(t_token **token, t_token *del);

/*
** print.c
*/

void	print_cmd(t_cmd *cmd);
void	print_token(t_token *token);

/*
** handler.c
*/

t_env	*handler(int opt, char **env, char *name, char *val);

/*
** echo.c
*/

int		ex_echo(t_cmd *cmd);
void	ex_env(t_cmd *cmd);
void    ex_port(t_cmd *cmd);
void    ex_unset(t_cmd *cmd);

/*
** exec.c
*/

void	child(t_cmd *cmd);
char	**exec(t_cmd *cmd);

/*
** path.c
*/

int	ex_cd(t_cmd *cmd);
int	ex_pwd(t_cmd *cmd);

/*
** utils.c
*/

char	**get_env(t_env *env);
char	*get_value(char *ret);
char	*get_name(char *ret);

#endif
