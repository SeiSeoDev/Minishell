/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:38:39 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/17 11:14:56 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>

# define STDIN		0
# define STDOUT		1
# define READ_END	0
# define WRTIE_END	1

typedef enum s_type
{
	word,
	fd,
	lim,
	pip,
	rin,
	rdin,
	rout,
	rdout,
}	t_type;

/*	FOR EASY PARSING
**	typedef enum s_type
**	{
**		word, "whatever"
**		fd, >> "this"
**		lim, << "this"
**		pip, |
**		rin, <
**		rdin, <<
**		rout, >
**		rdout, >>
**	}	t_type;
*/

typedef struct s_token
{
	int				type;
	char			*str;
	int				fd;
	struct s_token	*next;
}	t_token;

/* TO PARS THE INSTRUCTIONS
**	typedef struct s_token
**	{
**		int				type; t_type for parsing
**		char			*str; "str" that need to be expend depending on type
**		int				fd;	for signal across child and parents
**		struct s_token	*next;
**	}	t_token;
*/

typedef struct s_cmd
{
	t_token			*arg;
	t_token			*redir;
	int				fdin;
	int				fdout;
	int				pid;
	struct s_cmd	*next;
}	t_cmd;

/*	TO CREAT A COMMAND
**	typedef struct s_cmd
**	{
**		t_token			*arg; while arg exec(arg) arg = arg->next
**		t_token			*redir; after > | < | >> | <<
**		int				fdin; a remplir avec l entree
**		int				fdout; ou est ce que j ecris/transmet
**		int				pid; to kill process
**		struct s_cmd	*next;
**	}	t_cmd;
*/

typedef struct s_env
{
	char			*name;
	char			*val;
	struct s_env	*next;
}	t_env;

/*	TO BUILT ENV
**	typedef struct s_env
**	{
**		char			*name; name of the env var
**		char			*val; value of this env var
**		struct s_env	*next;
**	}	t_env;
*/

/*
** free.c
*/

void	free_token(t_token *token);
void	free_cmd(t_cmd *cmd);
void	free_env(t_env *env);
void	free_tab(char **tab);
void	ctrfree(void *ptr, char *err, char type, int num);
void	exfree(void *ptr, char *err, char typ, int num);

/*
** usefull_pars.c
*/

void	skip_cot(char *str, int *i);
void	split(char *str, int *i, int *last);
void	get_type(t_token *tmp, int *l, int *f);
int		quot_status(char *str, int i);
t_token	*token_syntax(t_token *token);

/*
** expend.c
*/

char	*del_unused_quot(char *str);
char	*expend_words(char *str);

/*
** pars.c
*/

void	split_words(char *str);

/*
** init.c
*/

t_env	*init_env(t_env *next, char *name, char *val);
t_token	*init_token(t_token *next, char *str, int type);
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
void	ex_port(t_cmd *cmd);
void	ex_unset(t_cmd *cmd);

/*
** exec.c
*/

void	exec(t_cmd *cmd);
void	ex_hit(t_cmd *cmd);

/*
** path.c
*/

int		ex_cd(t_cmd *cmd);
int		ex_pwd(t_cmd *cmd);

/*
** utils.c
*/

char	**get_env(t_env *env);
char	*get_value(char *ret);
char	*get_name(char *ret);

/*
** cmd.c
*/

t_cmd	*parsing_error(t_cmd *cmd);
t_token	*cmd_arg(t_token **tmp);
t_token	*cmd_redir(t_token **tmp);

/*
** redir.c
*/

int		find_file(char *path);
char	*fill_fd(t_cmd *cmd);
void	close_fd(t_cmd *cmd);
int		isntopen(t_cmd *cmd);

/*
** fork.c
*/

int		is_built(t_cmd *cmd);
void	child(t_cmd *cmd);
void	define_sig(int isparent);
void	sig_handler(int sig);
void	sig_handler2(int sig);

t_cmd	*cmd_init(t_cmd *res, t_token **tmp, t_token *token);
void	cmd_add(t_token **tmp, t_cmd *data);

#endif
