/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/27 14:25:36 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int find_file(char *path)
{
	struct stat	sb;
	//int			res;

	if (!path)
		return (0);
//		printf("%s <pathname> : ", path);
	if (lstat(path, &sb) == -1)
	{
	//	printf("File does not exist\n");
		return (0);
	}
	//printf("Type de fichier : ");
	/*res = (sb.st_mode & S_IFMT);
	if (res == S_IFBLK)
		printf("périphérique de bloc\n");
	else if (res == S_IFCHR)
		printf("périphérique de caractère\n");
	else if (res == S_IFDIR)
		printf("répertoire\n");
	else if (res == S_IFIFO)
		printf("FIFO/tube\n");
	else if (res == S_IFLNK)
		printf("lien symbolique\n");
	else if (res == S_IFREG)
		printf("fichier ordinaire\n");
	else if (res == S_IFSOCK)
		printf("socket\n");
	else
		printf("inconnu ?\n");*/
	return (1);
}

static char	**creat_arg(t_cmd *cmd)
{
	t_token	*tmp;
	char	**arg;
	int		i;

	tmp = cmd->arg;
	i = 0;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	arg = malloc(sizeof(char *) * (i + 1));
	if (!arg)
		return (NULL);
	tmp = cmd->arg;
	i = 0;
	while (tmp)
	{
		arg[i++] = tmp->str;
		tmp = tmp->next;
	}
	arg[i] = NULL;
	return (arg);
}

static char *creat_exe(t_env *env, t_cmd *cmd)
{
	char	*path;
	char	*exe;
	int		i;
	int		j;
	
	if (!env->val)
		return (NULL);
	i = 0;
	j = i;
	while (env->val[i])
	{
		if (env->val[i] == ':')
		{
			path = ft_strndup(&env->val[j], i - j);
			if (!path)
				return (NULL);
			path = ft_free_join(path, "/", 1);
			if (!path)
				return (NULL);
			exe = ft_strjoin(path, cmd->arg->str);
			free(path);
			if (find_file(exe))
				return (exe);
			free(exe);
			j = i + 1;
		}
		i++;
	}
	return (NULL);
}

static int	exe_prog(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;
	char	**all;

	env = handler(3, NULL, NULL, NULL);
	if (!env)
		return (0);
	all = get_env(env);
	if (!all)
		return (0);
	env = handler(3, NULL, "PWD", NULL);
	exe = ft_strjoin(env->val, &cmd->arg->str[1]);
	arg = creat_arg(cmd);
	if (!env || !exe || !arg || !(find_file(exe)))
	{
		if (exe)
			free(exe);
		if (arg)
			free(arg);
		free_tab(all);
		return (0);
	}
	if (!ft_strcmp(cmd->arg->str, "./minishell"))
		handler(3, NULL, "SHLVL", ft_itoa(ft_atoi(handler(3, NULL, "SHLVL", NULL)->val) + 1));
	if 	(execve(exe, arg, all) == -1)
		printf("command failed\n");
	free(arg);
	free(exe);
	free_tab(all);
	return (1);
}

static int	exe_cmd(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;
	char	**all;

	env = handler(3, NULL, "PATH", NULL);
	if (!env)
		return (0);
	exe = creat_exe(env, cmd);
	if (!exe)
	{
		printf("deadly ?\n");
		return (0);
	}
	arg = creat_arg(cmd);
	env = handler(3, NULL, NULL, NULL);
	if (!env)
		return (0);
	all = get_env(env);
	if (!arg || !all)
	{
		free(exe);
		if (all)
			free_tab(all);
		if (arg)
			free(arg);
		return (0);
	}
	if 	(execve(exe, arg, all) == -1)
	{
		printf("command failed\n");
	}
	if (arg)
		free(arg);
	if (exe)
		free(exe);
	if (all)
		free(all);
	printf("end\n");
	return (1);
}

void	heredoc(t_cmd *cmd, t_token *redir)
{
	char	*str;
	t_token	*tmp;
	int		quot;

	tmp = cmd->arg;
	while (tmp->next)
		tmp = tmp->next;
	quot = 0;
	if (ft_strchr(redir->str, '"') || ft_strchr(redir->str, '\''))
		quot = 1;
	redir->str = del_unused_quot(redir->str);
	str = readline("\e[1m\e[31m\002"">""\001\e[0m\002");
	while (ft_strcmp(str, redir->str))
	{
		if (quot)
			tmp->next = init_token(NULL, expend_words(tmp, str), 0);
		else
			tmp->next = init_token(NULL, ft_strdup(str), 0);
		tmp = tmp->next;
		str = readline("\e[1m\e[31m\002"">""\001\e[0m\002");
	}
	tmp = cmd->arg;
	while (tmp)
	{
		printf("token: str=%s, type=%d\n", tmp->str, tmp->type);
		tmp = tmp->next;
	}
}

void	fill_fd(t_cmd *cmd)
{
	int	opout;
	int	opin;
	t_token *token;

	token = cmd->redir;
	opout = 0;
	opin = 0;
	while (token)
	{
		if (token->type == rout)
		{
			cmd->fdout = open(token->next->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			printf("open fd : %d\n", cmd->fdout);
			token->fd = cmd->fdout;
		}
		else if (token->type == rdout)
		{	
			cmd->fdout = open(token->next->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
			token->fd = cmd->fdout;
		}
		else if (token->type == rin)
		{
			cmd->fdin = open(token->next->str,  O_RDONLY);
			token->fd = cmd->fdin;
		}
		else if (token->type == rdin)
		{
			heredoc(cmd, token->next);
		}
		token = token->next;
	}
}

void    cleartest(t_cmd *cmd)
{
	int i;
	char **all;
	(void)cmd;
	i = 0;
	all = get_env(handler(3, NULL, NULL, NULL));
	while (all[i])
	{
		write(cmd->fdout, all[i], ft_strlen(all[i]));
		write(cmd->fdout, "\n", 1);
		i++;
	}
}

void	close_fd(t_cmd *cmd)
{
	t_token *token;

	token = cmd->redir;
	while (token)
	{
		if (token->fd != 1 && token->fd != 0)
		{
			close(token->fd);
			printf("close fd -> %d", token->fd);
		}
		token = token->next;
	}
}

char    **exec(t_cmd *cmd)
{
	if (cmd->redir)
		fill_fd(cmd);
	dup2(fd_in, STDIN_FILENO);
	if (cmd != NULL && cmd->arg != NULL)
	{

		if (!ft_strcmp(cmd->arg->str, "echo"))
			ex_echo(cmd);
		else if (!ft_strcmp(cmd->arg->str, "cd"))
		{
			ex_cd(cmd);
		}
		else if (!ft_strcmp(cmd->arg->str, "pwd"))
			ex_pwd(cmd);
		else if (!ft_strcmp(cmd->arg->str, "env"))
			ex_env(cmd);
		else if (!ft_strcmp(cmd->arg->str, "unset"))
			ex_unset(cmd);
		else if (!ft_strcmp(cmd->arg->str, "export"))
			ex_port(cmd);
		else
		{
			if (!ft_strncmp(cmd->arg->str, "./", 2))
				exe_prog(cmd);
			else if (!exe_cmd(cmd))
				printf("Minishell: %s: command not found\n", cmd->arg->str);
		}
	}
	printf("fdout : %d\n", cmd->fdout);
	close_fd(cmd);
	return (get_env(handler(3, NULL, NULL, NULL)));
}

int		get_nbpipe(t_cmd *cmd)
{
	int i;
	t_cmd *tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int		is_built(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->arg->str, "echo"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "cd"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "env"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "unset"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "export"))
		return (1);
	return (0);
}

void	child(t_cmd *cmd)
{
	int		*pipefd;
	int		*pitab;
	int		i;
	int		fd_in;
	int		status;
	t_cmd	*tmp;
	
	tmp = cmd;
	pipefd = malloc((get_nbpipe(cmd) * 2) * sizeof(int));
	pitab = malloc((get_nbpipe(cmd)) * sizeof(int));
	fd_in = dup(STDIN_FILENO);
	i = 0;
	if (get_nbpipe(cmd) == 1 && is_built(cmd))
	{
		exec(cmd);
		return;
	}
	while (i < get_nbpipe(cmd))
	{
		pipe(&pipefd[i * 2]);
		pitab[i] = fork();
		if (pitab[i] == 0)
		{
			if (i != 0)
				dup2(fd_in, STDIN_FILENO);
			if ((i + 1) != get_nbpipe(cmd))
				dup2(pipefd[i * 2 + 1], STDOUT_FILENO);
			close(pipefd[i * 2]);
			close(pipefd[i * 2 + 1]);
			close(fd_in);
			exec(tmp);
			exit_free(cmd, NULL, 'c');

		}
		dup2(pipefd[i * 2], fd_in);
		close(pipefd[i * 2]);
		close(pipefd[i * 2 + 1]);
		i++;
		tmp = tmp->next;
	}
	close(fd_in);
    i = -1;
    while (++i < get_nbpipe(cmd))
		waitpid(pitab[i], &status, 0);
	free_cmd(cmd);
	free(pipefd);
	free(pitab);
}
