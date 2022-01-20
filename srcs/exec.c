/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/20 18:33:22 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <time.h>

static int find_file(char *path)
{
	struct stat sb;
	int res;

	if (path)
		printf("%s <pathname> : ", path);
	if (lstat(path, &sb) == -1)
	{
		printf("File does not exist\n");
		return (0);
	}
	printf("Type de fichier : ");
	res = (sb.st_mode & S_IFMT);
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
		printf("inconnu ?\n");
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
		handler(3, NULL, "SHLVL", "2");
	if 	(execve(exe, arg, all) == -1)
		printf("command failed\n");
	free(arg);
	free(exe);
	free_tab(all);
	printf("end\n");
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

void	fill_fd(t_cmd *cmd)
{
	int	opout;
	int	opin;

	printf ("REDIR : %s\n", cmd->redir->str);
	printf ("INITIAL FDOUT : %d\n", cmd->fdout);
	opout = 0;
	if (cmd->redir->type == rout)
	{
		if (opout == 1)
			close(cmd->fdout);
		cmd->fdout = open(cmd->redir->next->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		opout = 1;
	}
	else if (cmd->redir->type == rdout)
	{	
		if (opout == 1)
			close(cmd->fdout);
		cmd->fdout = open(cmd->redir->next->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
		opout = 1;
	}
	printf ("FINAL FDOUT : %d\n", cmd->fdout);
	printf ("REDIR : %s\n", cmd->redir->str);
	printf ("INITIAL FDIN : %d\n", cmd->fdout);
	opin = 0;
	if (cmd->redir->type == rin)
	{
		if (opin == 1)
			close(cmd->fdin);
		cmd->fdin = open(cmd->redir->next->str,  O_RDONLY);
		opin = 1;
	}
//	else if (cmd->redir->type == rin)
//	{
//		if (opin == 1)
//			close(cmd->fdin);
//		cmd->fdin = open(,  O_RDWR);
//		opin = 1;
//	}
	printf ("FINAL FDIN : %d\n", cmd->fdout);
}

void    exec(t_cmd *cmd)
{
	if (cmd->redir)
		fill_fd(cmd);
	if (cmd != NULL && cmd->arg != NULL)
	{
		printf("TEST : %s\n", cmd->arg->str);
		if (!ft_strcmp(cmd->arg->str, "echo"))
			ex_echo(cmd);
		else if (!ft_strcmp(cmd->arg->str, "cd"))
			ex_cd(cmd);
		else if (!ft_strcmp(cmd->arg->str, "pwd"))
			ex_pwd(cmd);
		else if (!ft_strcmp(cmd->arg->str, "env"))
			ex_env(cmd);
		else if (!ft_strcmp(cmd->arg->str, "unset"))
			ex_unset(cmd);
		else
		{
			if (!ft_strncmp(cmd->arg->str, "./", 2))
				exe_prog(cmd);
			else if (!exe_cmd(cmd))
				printf("Minishell: %s: command not found\n", cmd->arg->str);
		}
	}
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
			exit_free(cmd, "child end", 'c');
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
	printf("father end\n");
}
