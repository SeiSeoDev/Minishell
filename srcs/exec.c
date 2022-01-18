/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/18 17:36:42 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <time.h>

static int find_file(char *path)
{
	struct stat sb;
	int res;

	if (path)
		printf("%s <pathname>\n", path);
	if (stat(path, &sb) == -1)
		return (0);
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
	{
		printf("inconnu ?\n");
		return (0);
	}
	// printf("Numéro d'inœud :            %ld\n", (long)sb.st_ino);
	// printf("Mode :                      %lo (octal)\n", (unsigned long)sb.st_mode);
	// printf("Nombre de liens :           %ld\n", (long)sb.st_nlink);
	// printf("Propriétaires :                    UID=%ld   GID=%ld\n", (long)sb.st_uid, (long)sb.st_gid);
	// printf("Taille de bloc d’E/S :      %ld octets\n", (long)sb.st_blksize);
	// printf("Taille du fichier :         %lld octets\n", (long long)sb.st_size);
	// printf("Blocs alloués :             %lld\n", (long long)sb.st_blocks);
	// printf("Dernier changement d’état : %s", ctime(&sb.st_ctime));
	// printf("Dernier accès au fichier :  %s", ctime(&sb.st_atime));
	// printf("Dernière modif du fichier:  %s", ctime(&sb.st_mtime));
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
			path = ft_free_join(path, "/", 1);
			printf("path=%s\n", path);
			printf("arg=%s\n", cmd->arg->str);
			exe = ft_strjoin(path, cmd->arg->str);
			free(path);
			path = NULL;
			if (find_file(exe))
				return (exe);
			j = i + 1;
		}
		i++;
	}
	return (NULL);
}

static int	exe_cmd(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;

	env = handler(3, NULL, "PATH", NULL);
	if (!env)
		return (0);
	arg = creat_arg(cmd);
	exe = creat_exe(env, cmd);
	if 	(execve(exe, arg, env->all) == -1)
	{
		printf("command failed\n");
	}
	else
	{
		printf("command success\n");
	}
	if (arg)
		free(arg);
	if (exe)
		free(exe);
	return (0);
}

void    exec(t_cmd *cmd)
{
	if (cmd != NULL && cmd->arg != NULL)
	{
		printf("TEST : %s\n", cmd->arg->str);
		if (!ft_strcmp(cmd->arg->str, "echo"))
			ex_echo(cmd);
		else if (!ft_strcmp(cmd->arg->str, "cd"))
			ex_cd(cmd);
		else if (!ft_strcmp(cmd->arg->str, "pwd"))
			ex_pwd(cmd);
		else
		{
			if (!exe_cmd(cmd))
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
	int        nb_pipe;
	nb_pipe = get_nbpipe(cmd);
	printf("ON A %d pipes\n", nb_pipe);

    int        *fd = malloc(sizeof(*fd) * 2 * nb_pipe);
    pid_t    pid;
    int        status;
	t_cmd	*tmp;
	tmp = cmd;
	
    for (int i = 0; i < nb_pipe; ++i)
    {
        pipe(&fd[i * 2]);
        pid = fork();
        if (pid == 0)
        {
            if (i != nb_pipe - 1)
                dup2(fd[i * 2 + 1], STDOUT); //dup write end in stdout
            if (i > 0)
                dup2(fd[(i - 1) * 2], STDIN); //dup read end of previous pipe in stdin
			//printf("ON ME VOIT : %s\n", tmp->arg->str);
            exec(tmp);
            exit(1);
        }
        else
        {
            waitpid(pid, &status, 0);
            close(fd[i * 2 + 1]); //close write end
            if (i == nb_pipe - 1)
                close(fd[i * 2]); //close read end
            if (i > 0)
                close(fd[(i - 1) * 2]); //close read end of previous pipe
        }
		tmp = tmp->next;
    }

}